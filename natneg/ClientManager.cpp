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

#include <TemplateStringServer.h>

void CClientManager::Free()
{
	ClientMap::iterator it = m_clients.begin();

	while (it != m_clients.end())
	{
		// Free CClient memory
		if (it->second)
			delete it->second;

		it++;
	}

	m_clients.clear();
}

void CClientManager::Delete(ClientMap::iterator it)
{
	CTemplateServer* server = NULL;

	if (it == m_clients.end())
		return;

	if (!it->second)
		return;

	delete (it->second);

	m_clients.erase(it);
}

bool CClientManager::CreateAndHandle(CDatabase* db, mdk_socket stream, const struct sockaddr_in* addr_in, const char *data, int len)
{
	// Create che client
	CClient* c = new CClient(stream, addr_in, db);
	SClientStruct ss;
	
	inet_ntop(AF_INET, &(addr_in->sin_addr), ss.ip, INET_ADDRSTRLEN);

	ss.port = addr_in->sin_port;	
	
	m_clients[ss] = c;
	
	if (!c->Handle(data, len))
	{
		Delete(c); // Delete the client if it returned false
		return false;
	}

	return true;
}

bool CClientManager::Handle(CDatabase* con, mdk_socket stream, const struct sockaddr* addr, const char *data, int len)
{
	ClientMap::iterator it;
	unsigned int i = 0;
	SClientStruct ss;
	struct sockaddr_in* addr_in = (struct sockaddr_in*)addr;
	
	inet_ntop(AF_INET, &(addr_in->sin_addr), ss.ip, INET_ADDRSTRLEN);
	ss.port = addr_in->sin_port;
	
	it = m_clients.find(ss);

	if (it == m_clients.end())
		return CreateAndHandle(con, stream, addr_in, data, len);

	// Handle our client
	if (!it->second->Handle(data, len))
	{
		Delete(it); // Delete the client if it returned false
		return false;
	}

	return true;
}

void CClientManager::Delete(CClient* c)
{
	ClientMap::iterator it = m_clients.begin();

	while (it != m_clients.end())
	{
		CClient *cc = it->second;

		if (cc == c)
		{
			Delete(it);
			return;
		}
		
		it++;
	}	
}

CClient* CClientManager::FindClientByCookieIndex(int cookie, unsigned char index)
{
	ClientMap::iterator it = m_clients.begin();

	while (it != m_clients.end())
	{
		CClient *cc = it->second;

		if (cc->GetCookie() == cookie && cc->GetClientIndex() == index)
			return cc;
		
		it++;
	}	
}


ClientMap CClientManager::m_clients;
