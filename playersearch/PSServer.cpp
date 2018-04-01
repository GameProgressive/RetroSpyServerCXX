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
#include "PSServer.h"

#include "../common/RSString.h"

PSServer::PSServer(CLoop *loop) : CStringServer(loop) {}
PSServer::~PSServer() {}

bool PSServer::HandleRequest(uv_stream_t *stream, const char *req, const char *buf, int size)
{
	if (_stricmp(req, "valid") == 0)
		return _OnValid(stream, buf, size);

	printf("[PlayerSearch] Unknown request: %s\n", req);
	return false;
}

// Unused
bool PSServer::OnNewConnection(uv_stream_t*) { return true; }

bool PSServer::_OnValid(uv_stream_t *, const char *buf, int)
{
	printf("[PlayerSearch] RECV: %s\n", buf);
	return true;
}

