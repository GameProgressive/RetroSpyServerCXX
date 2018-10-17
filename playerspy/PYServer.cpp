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

#include <MDK/ModuleEntryPoint.h>
#include <MDK/Utility.h>

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

PYServer::PYServer(int defaultport, bool ip) : CTemplateStringServer(defaultport, ip) {}

int PYServer::Initialize()
{
	if (!m_lpDatabase)
		return ERROR_DATABASE_ERROR;
	
	PYServer::server_id = 1;
	strrand(PYServer::server_challenge, sizeof(PYServer::server_challenge), "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	
	return ERROR_NONE;
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
		WriteTCP(stream, "\\ka\\final\\");
		return true;
	}
	else if (strcasecmp(req, "newuser") == 0)
	{
		return HandleNewUser(stream, buf, size);
	}

	if (!CClientManager::Handle(m_lpDatabase, stream, req, buf, size))
	{
		return false;
	}
	
	return true;
}

bool PYServer::HandleNewUser(mdk_socket stream, const char* buf, int size)
{
	char nick[GP_NICK_LEN], email[GP_EMAIL_LEN], pass[GP_PASSWORD_LEN], passenc[GP_PASSWORDENC_LEN], unick[GP_UNIQUENICK_LEN];
	
	pass[0] = nick[0] = email[0] = passenc[0] = unick[0] = '\0';
	
	
	if (!get_gs_data(buf, "uniquenick", unick, GP_UNIQUENICK_LEN))
		return false;
	
	if (!get_gs_data(buf, "email", email, GP_EMAIL_LEN))
		return false;
	
	if (!get_gs_data(buf, "nick", nick, GP_NICK_LEN))
		return false;
	
	if (!get_gs_data(buf, "passenc", passenc, GP_PASSWORDENC_LEN))
		return false;

	gs_pass_decode(passenc, pass);
	
	
	
	return true;
}

bool PYServer::OnTCPNewConnection(mdk_socket stream, int)
{
	char fch[45];
	fch[0] = 0;

	snprintf(fch, sizeof(fch), "\\lc\\1\\challenge\\%s\\id\\%d\\final\\", server_challenge, server_id);

	WriteTCP(stream, fch);
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

ModuleEntryPoint(PYServer, 29900, false)
