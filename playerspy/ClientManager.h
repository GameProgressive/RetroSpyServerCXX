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
#ifndef PY_CLIENTMANAGER_H
#define PY_CLIENTMANAGER_H

#include "Client.h"

#include <map>

typedef std::map<unsigned int, CClient*> ClientMap;

class CClientManager
{
public:
	CClientManager(const char *server_challenge, int server_id);
	~CClientManager();

	bool Handle(uv_stream_t* stream, const char *req, const char*buf, int len);

private:
	ClientMap m_clients;

	int m_serverid;
	char *m_challenge;

private:
	void Delete(ClientMap::iterator it);
	bool CreateAndHandle(uv_stream_t* stream, const char *req, const char *buf, int len);
	
};

#endif
