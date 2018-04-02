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
#include "ClientManager.h"

#include "../common/Server.h" //CClientData

CClientManager::CClientManager(const char *server_chall, int server_id)
{
	m_challenge = (char*)server_chall;
	m_serverid = server_id;
}

CClientManager::~CClientManager()
{
	ClientMap::iterator it = m_clients.begin();

	while (it != m_clients.end())
	{
		// Free CClient memory
		if (it->second)
			delete it->second;

		m_clients.erase(it);
	}
}

void CClientManager::Delete(ClientMap::iterator it)
{
	if (it == m_clients.end())
		return;

	if (it->second)
	{
		delete it->second;
	}

	m_clients.erase(it);
}

bool CClientManager::CreateAndHandle(uv_stream_t *stream, const char *req, const char *buf, int len)
{
	ClientMap::iterator it;

	CClientData *cc = (CClientData*)stream->data;

	unsigned int i = m_clients.size();
	
	// Create che client
	m_clients[i] = new CClient(stream);
	
	// Go to the position so we can pass the key memory
	it = m_clients.begin();
	std::advance(it, i);

	// Pass the key memory (no corruptions ^.^)
	cc->SetUserData((void*)&it->first);

	if (!it->second->Handle(req, buf, len))
	{
		Delete(it); // Delete the client if it returned false
		return false;
	}

	return true;
}

bool CClientManager::Handle(uv_stream_t* stream, const char *req, const char*buf, int len)
{
	CClientData *cc = (CClientData*)stream->data;
	ClientMap::iterator it;

	if (!cc->GetUserData())
		return CreateAndHandle(stream, req, buf, len); // Create the instance

	it = m_clients.find(*((unsigned int *)cc->GetUserData()));

	// Invalid number, disconnect the client
	if (it == m_clients.end())
	{
		cc->SetUserData(NULL);
		return false;
	}

	// Handle our client
	if (!it->second->Handle(req, buf, len))
	{
		Delete(it); // Delete the client if it returned false
		return false;
	}

	return true;
}
