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

#include "../common/RSString.h"
#include "../common/Query.h"
#include "../common/Helper.h"

#include <string.h>
#include <stdio.h>

PYServer::PYServer(CLoop *loop) : CStringServer(loop)
{
	server_id = 1;
	
	strrnd(server_challenge, sizeof(server_challenge), "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	
	cm = new CClientManager(server_challenge, server_id);
}

PYServer::~PYServer()
{
	if (cm)
		delete cm;

	cm = NULL;
}

bool PYServer::HandleRequest(uv_stream_t *stream, const char *req, const char *buf, int size)
{
	if (_stricmp(req, "ka") == 0)
	{
		// Keep-Alive
		Write(stream, "\\ka\\final\\");
		return true;
	}

	cm->Handle(stream, req, buf, size);
	return true;
}

bool PYServer::OnNewConnection(uv_stream_t* stream)
{
	char fch[45];
	fch[0] = 0;

	_snprintf_s(fch, sizeof(fch), sizeof(fch)-1, "\\lc\\1\\challenge\\%s\\id\\%d\\final\\", server_challenge, server_id);

	Write(stream, fch);
	return true;

}
