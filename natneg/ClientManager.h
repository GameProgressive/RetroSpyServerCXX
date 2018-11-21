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
#ifndef NN_CLIENTMANAGER_H
#define NN_CLIENTMANAGER_H

#include "Client.h"

#include <MDK/TemplateServer.h>

#include <map>

struct SClientStruct
{
	char ip[INET_ADDRSTRLEN];
	unsigned short port;
};

static inline bool operator< (const SClientStruct s1, const SClientStruct& s2)
{
	return s1.port < s2.port;
}

typedef std::map<SClientStruct, CClient*> ClientMap;

class CClientManager
{
public:
	static bool Handle(CDatabase* db, mdk_socket stream, const struct sockaddr* addr, const char*data, ssize_t len);
	static void Free();

	static CClient* FindClientByCookieIndex(int cookie, unsigned char index);
	
	static void Delete(CClient* c);
	
private:
	static ClientMap m_clients;

private:
	static void Delete(ClientMap::iterator ss);
	static bool CreateAndHandle(CDatabase* db, mdk_socket stream, const struct sockaddr_in* addr, const char *data, ssize_t len);
};

#endif
