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
#include "PeerChatServer.h"

#include "Cache.h"

#include <MDK/ModuleEntryPoint.h>
#include <MDK/Utility.h>

int PeerChatServer::Initialize()
{
	if (!m_lpDatabase)
		return ERROR_DATABASE_ERROR;
	
	return ERROR_NONE;
}

PeerChatServer::PeerChatServer(int defaultport, bool udp) : CThreadServer(defaultport, udp)
{
	m_cache = new CCache();
}

PeerChatServer::~PeerChatServer()
{
	if (m_cache)
		delete m_cache;
	
	m_cache = NULL;
}

bool PeerChatServer::OnTCPNewConnection(mdk_socket stream, int status)
{
	if (m_cache->AddUser(stream))
		return false;
	
	return true;
}

void PeerChatServer::OnTCPRead(mdk_socket client, const char *data, ssize_t size)
{
	if (GetSocketExtraData(client)->GetData() == NULL)
	{
		// No client pointer!
		Close(client);
		return;
	}
	
	((CIRCClient*)GetSocketExtraData(client)->GetData())->Process(data, size);
}


ModuleEntryPoint(PeerChatServer, 6667, false)
