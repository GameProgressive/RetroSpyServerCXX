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
#ifndef NNSERVER_H
#define NNSERVER_H

#include <MDK/ThreadServer.h>

#include <string.h>

/*
	This class rappresents a Player Search server
*/
class CNNServer : public CThreadServer
{
public:
	CNNServer(int defaultport, bool udp);
	~CNNServer();
	
	int Initialize();

	bool OnTCPNewConnection(mdk_socket stream, int status) { return true; }

	void OnTCPRead(mdk_socket client, const char *data, ssize_t size) {}
	void OnUDPRead(mdk_socket client, const struct sockaddr* addr, const char *data, ssize_t size);

	
	static const char* GetMatchIP() { return m_matchIp; }
	static const char* GetProbeIP() {  return m_probeIp; }
	
	void SetMatchIP(const char* ip) { strncpy(m_matchIp, ip, INET_ADDRSTRLEN); }
	void SetProbeIP(const char* ip) { strncpy(m_probeIp, ip, INET_ADDRSTRLEN); }
private:
	CDatabase* m_dbConnection;

	static char m_matchIp[INET_ADDRSTRLEN];
	static char m_probeIp[INET_ADDRSTRLEN];
};

#endif
