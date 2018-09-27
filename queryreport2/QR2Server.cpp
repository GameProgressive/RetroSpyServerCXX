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
#include "QR2Server.h"

#define PACKET_TYPE    0x09

#include <MDK/ModuleEntryPoint.h>
#include <MDK/Utility.h>

QR2Server::QR2Server(int defaultport, bool udp) : CThreadServer(defaultport, udp) {}

QR2Server::~QR2Server()
{
}

void QR2Server::OnUDPRead(mdk_socket client, const struct sockaddr* addr, const char *data, ssize_t size)
{
	unsigned char newdata[7];
	
	if (size < 6)
		return;
	
	if (data[0]!=PACKET_TYPE)
		return;
	
	// Packet check
	newdata[0] = 0xFE;
	newdata[1] = 0xFD;
	newdata[2] = 0x09;
	
	// Status check (int=sizeof(4))
	newdata[3] = 0x00; //0x00 if avaiable, 0x01 if unavaible, 0x02 if unavaiable temporarily
	newdata[4] = 0x00;
	newdata[5] = 0x00;
	newdata[6] = 0x00;
	
	WriteUDP(client, newdata, sizeof(newdata), addr);
}

int QR2Server::Initialize()
{
	if (!m_lpDatabase)
		return ERROR_DATABASE_ERROR;
	
	return ERROR_NONE;
}

ModuleEntryPoint(QR2Server, 27900, true)
