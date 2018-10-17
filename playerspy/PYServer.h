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
#ifndef GPCMSERVER_H
#define GPCMSERVER_H

#include <Defines.h>
#include <TemplateStringServer.h>
#include <MDK/Database.h>

/*
	This class rappresents a Player Search server
*/
class PYServer : public CTemplateStringServer
{
public:
	PYServer(int defaultport, bool udp);
	~PYServer();
	
	int Initialize();

	/* See CServer::OnNewConnection */
	bool OnTCPNewConnection(mdk_socket stream, int status);

	/* See CStringServer::HandleRequest */
	bool HandleRequest(mdk_socket stream, const char *req, const char *buf, int size);

	static int GetServerID();
	static const char *GetServerChallenge();

private:
	static char server_challenge[GP_SERVERCHALL_LEN];
	static int server_id;
	
	bool HandleNewUser(mdk_socket stream, const char* buf, int size);
};

#endif
