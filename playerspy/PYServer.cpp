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
#include "PYServer.h"

#include "ClientManager.h"

#include <Helper.h>

#include <string.h>
#include <stdio.h>

#include <MDK/Utility.h>

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

PYServer::PYServer(CDatabase* db)
{
	PYServer::server_id = 1;
	strrand(PYServer::server_challenge, sizeof(PYServer::server_challenge), "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	m_dbConnection = db;
}

PYServer::~PYServer()
{
	CClientManager::Free();
}

bool PYServer::HandleRequest(mdk_socket stream, const char *req, const char *buf, int size)
{
	if (strcasecmp(req, "ka") == 0)
	{
		// Keep-Alive
		Write(stream, "\\ka\\final\\");
		return true;
	}

	if (!CClientManager::Handle(m_dbConnection, stream, req, buf, size))
	{
		// Write something for tell the client that something happend
		Write(stream, "\\err\\CLIENT_HANDLE_FAILED\\final\\");
		return false;
	}
	
	return true;
}

bool PYServer::OnNewConnection(mdk_socket stream, int)
{
	char fch[45];
	fch[0] = 0;

	snprintf(fch, sizeof(fch), "\\lc\\1\\challenge\\%s\\id\\%d\\final\\", server_challenge, server_id);

	Write(stream, fch);
	return true;

}

int PYServer::GetServerID()
{
	return server_id;
}

const char *PYServer::GetServerChallenge()
{
	return server_challenge;
}

int PYServer::server_id = 0;
char PYServer::server_challenge[GP_SERVERCHALL_LEN] = {0};
