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
#ifndef NN_CLIENT_H
#define NN_CLIENT_H

#include <MDK/Database.h>
#include <MDK/TemplateSocket.h>

#include "NNTypes.h"

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <netinet/in.h> 
#endif

class CClient
{
public:
	CClient(mdk_socket stream, const struct sockaddr_in* addr, CDatabase* db);
	~CClient();

	bool Handle(const char *buf, ssize_t len);

	inline unsigned short GetPort() { return m_port; }
	inline const char* GetIP() { return m_ip; }

	inline mdk_socket GetSocket() { return m_stream; }
	
	inline struct sockaddr_in* GetAddr() { return &m_addr; }

	void Disconnect();

	// Operator overload
	bool operator==(CClient& c);
	bool operator!=(CClient& c);

	inline void SetConnectTime(time_t time) { m_connectTime = time; }
	inline void AllowConnection(bool c) { m_connected = c; }
	
	inline struct sockaddr_in GetPunchingAddr() { return m_punching; }
	
	inline bool IsInit() { return m_init; }
	inline bool HaveAck() { return m_ack; }
	
	inline int GetCookie() { return m_cookie; }
	inline unsigned char GetClientIndex() { return m_clientindex; }
	
private:
	struct sockaddr_in m_addr;
	struct sockaddr_in m_punching;
	time_t m_lastpacket, m_connectTime;
	mdk_socket m_stream;
	bool m_ack;
	int m_cookie;
	unsigned char m_clientindex;
	unsigned char m_version;
	bool m_init;
	unsigned int m_instance;
	bool m_connected;
	unsigned short m_port;
	char m_ip[INET_ADDRSTRLEN];
	
protected:
	bool HandleInitPacket(NatNegPacket* packet);
	bool HandleReport(NatNegPacket* packet);
	bool HandleAddressCheck(NatNegPacket* packet);
	bool HandleNatifyRequest(NatNegPacket* packet);
	void SendConnect(NatNegPacket* packet, bool sendToOther = true);
	
private:
	void Write(void* data, int len);
	
	CDatabase* m_dbConnect;
};

#endif
