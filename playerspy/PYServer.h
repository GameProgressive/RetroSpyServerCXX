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
#include <StringServer.h>

/*
	This class rappresents a Player Search server
*/
class PYServer : public CStringServer
{
public:
	PYServer(mdk_mysql con);
	~PYServer();

	/* See CServer::OnNewConnection */
	bool OnNewConnection(mdk_mysql stream);

	/* See CStringServer::HandleRequest */
	bool HandleRequest(mdk_mysql stream, const char *req, const char *buf, int size);

	static int GetServerID();
	static const char *GetServerChallenge();

private:
	static char server_challenge[GP_SERVERCHALL_LEN];
	static int server_id;
	mdk_mysql m_con;
};

#endif
