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
#ifndef PEERCHATSERVER_H
#define PEERCHATSERVER_H

#include <Defines.h>
#include <MDK/ThreadServer.h>
#include <MDK/Query.h>

/*
This class rappresents a Peer Chat Server
*/
class PeerChatServer : public CThreadServer
{
public:
	PeerChatServer(int defaultport, bool udp);
	~PeerChatServer();
	
	int Initialize();

	/* See CServer::OnNewConnection */
	bool OnTCPNewConnection(mdk_socket stream, int status) { return true; }

	void OnTCPRead(mdk_socket client, const char *data, ssize_t size) {}
	void OnUDPRead(mdk_socket client, const struct sockaddr* addr, const char *data, ssize_t size);
};

#endif
