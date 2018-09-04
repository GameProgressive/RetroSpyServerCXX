/*
    This file is part of RetroSpy Server.

    RetroSpy Server is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RetroSpy Server is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with RetroSpy Server.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Client.h"

#include "ClientManager.h"

#include <time.h>

#include "NNServer.h"

#include <MDK/Utility.h>

#ifndef _WIN32
#include <arpa/inet.h>
#endif

static unsigned char NNMagicData[] = {NN_MAGIC_0, NN_MAGIC_1, NN_MAGIC_2, NN_MAGIC_3, NN_MAGIC_4, NN_MAGIC_5};

CClient::~CClient()
{
	Disconnect();
}

CClient::CClient(mdk_socket stream, const struct sockaddr_in* addr, CDatabase* db)
{
	m_stream = stream;
	m_dbConnect = db;
	memcpy(&m_addr, addr, sizeof(struct sockaddr_in));
	memcpy(&m_punching, addr, sizeof(struct sockaddr_in));
	
	inet_ntop(AF_INET, &(addr->sin_addr), m_ip, INET_ADDRSTRLEN);
	m_port = addr->sin_port;
	
	m_lastpacket = 0;
	m_ack = false;
	m_lastpacket = time(NULL);
	m_version = 0;
	m_clientindex = 0;
	m_init = false;
	m_connectTime = time(NULL);
	m_connected = false;
}

void CClient::Disconnect()
{
	m_connected = false;
}

bool CClient::Handle(const char *buf, int len)
{
	NatNegPacket *packet = (NatNegPacket*)buf;
	if (memcmp(packet->magic, NNMagicData, NATNEG_MAGIC_LEN) != 0)
		return false;
	
	m_lastpacket = time(NULL);
	
	switch(packet->packettype)
	{
		case NN_INIT:
			return HandleInitPacket(packet);
		case NN_ADDRESS_CHECK:
			return HandleAddressCheck(packet);
		case NN_NATIFY_REQUEST:
			return HandleNatifyRequest(packet);
		case NN_CONNECT_ACK:
			m_ack = true;
			break;
		case NN_REPORT:
			return HandleReport(packet);
		default:
			LOG_WARN("NatNeg", "Unknown packet type: %d", packet->packettype);
			return false;
	}
	
	return true;
}

bool CClient::HandleInitPacket(NatNegPacket* packet)
{
	m_version = packet->version;
	
	if ((m_version > 1) && (packet->Packet.Init.porttype == 1))
	{
		CClient* c = CClientManager::FindClientByCookieIndex(packet->cookie, m_clientindex);
		if (c != NULL && c != this)
		{
			m_punching.sin_port = c->GetPunchingAddr().sin_port;
			CClientManager::Delete(c);
		}
	}
	
	m_cookie = packet->cookie;
	m_clientindex = packet->Packet.Init.clientindex;
	packet->packettype = NN_INITACK;
	Write(packet, INITPACKET_SIZE);
	
	if (packet->Packet.Init.porttype > 1)
		return false;
	
	m_init = true;
	
	if ((m_version < 2) || (packet->Packet.Init.porttype == 1))
		SendConnect(packet);
	
	return true;
}

void CClient::Write(void* data, int len)
{
	CTemplateSocket::WriteUDP(m_stream, data, len, (const struct sockaddr*)&m_addr);	
}

void CClient::SendConnect(NatNegPacket* packet, bool sendToOther)
{
	if (m_ack || m_init == false)
		return;
	
	CClient* c = CClientManager::FindClientByCookieIndex(packet->cookie, m_clientindex);
	if (c != NULL && c != this)
	{
		NatNegPacket sendpacket = {0};
		sendpacket.version = m_version;
		sendpacket.Packet.Connect.gotyourdata = 'B';
		sendpacket.packettype = NN_CONNECT;
		sendpacket.cookie = m_cookie;
		memcpy(sendpacket.magic, NNMagicData, NATNEG_MAGIC_LEN);
		sendpacket.Packet.Connect.finished = FINISHED_NOERROR;
		if (!c->IsInit() || c->HaveAck())
			return;
		
		c->AllowConnection(true);
		m_connected = true;
		
		sendpacket.Packet.Connect.remoteIP = c->GetPunchingAddr().sin_addr.s_addr;
		sendpacket.Packet.Connect.remotePort = c->GetPunchingAddr().sin_port;

		Write(&sendpacket, CONNECTPACKET_SIZE);
		
		m_connectTime = time(NULL);
		
		if(!sendToOther)
			return;		
		
		c->SetConnectTime(time(NULL));
		
		sendpacket.Packet.Connect.remoteIP = m_punching.sin_addr.s_addr;
		sendpacket.Packet.Connect.remotePort = m_punching.sin_port;
		
		Write(&sendpacket, CONNECTPACKET_SIZE);
	}
}

bool CClient::HandleAddressCheck(NatNegPacket* packet)
{
	packet->packettype = NN_ADDRESS_REPLY;
	packet->Packet.Init.localip = m_addr.sin_addr.s_addr;
	packet->Packet.Init.localport = m_addr.sin_port;
	Write(packet, INITPACKET_SIZE);
	return true;
}

bool CClient::HandleNatifyRequest(NatNegPacket* packet)
{
	int ret = 0, ertsocket = 0;
	struct sockaddr_in si, si_src;
	
	if (packet->Packet.Init.porttype == 1)
		Write(packet, INITPACKET_SIZE);
	
	ertsocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if (ertsocket == -1)
		return false;
	
	si.sin_family = AF_INET;
	si.sin_port = m_addr.sin_port;
	si.sin_addr.s_addr = m_addr.sin_addr.s_addr;
	si_src.sin_family = AF_INET;
	
	si_src.sin_port = htons(MATCHUP_PORT);
	if (packet->Packet.Init.porttype == NN_PT_NN2 || packet->Packet.Init.porttype == NN_PT_NN3)
	{
		si_src.sin_addr.s_addr = inet_addr(CNNServer::GetProbeIP());
	}
	else
		si_src.sin_addr.s_addr = inet_addr(CNNServer::GetMatchIP());
	
	if (packet->Packet.Init.porttype == NN_PT_NN3)
		ret = connect(ertsocket, (struct sockaddr*)&si_src, sizeof(si_src));
	else
		ret = bind(ertsocket, (struct sockaddr*)&si_src, sizeof(si_src));
	
	if (ret != 0)
		return false;
	
	sendto(ertsocket, (char*)packet, INITPACKET_SIZE, 0, (struct sockaddr*)&si, sizeof(si));
	close(ertsocket);

	return true;
}

bool CClient::HandleReport(NatNegPacket* packet)
{
	packet->packettype = NN_REPORT_ACK;
	Write(packet, REPORTPACKET_SIZE);
	return true;
}