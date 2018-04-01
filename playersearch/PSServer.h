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
#ifndef GPSPSERVER_H
#define GPSPSERVER_H

#include <StringServer.h>
#include <Loop.h>

/*
	This class rappresents a Player Search server
*/
class PSServer : public CStringServer
{
public:
	PSServer(CLoop* loop);
	~PSServer();

	/* See CServer::OnNewConnection */
	bool OnNewConnection(uv_stream_t* stream);

	/* See CStringServer::HandleRequest */
	bool HandleRequest(uv_stream_t *stream, const char *req, const char *buf, int size);

private:
	bool _OnValid(uv_stream_t *stream, const char *buf, int size);

};

#endif
