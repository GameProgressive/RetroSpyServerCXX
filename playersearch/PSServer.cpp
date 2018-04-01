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
#include "../common/Query.h"

#include <string.h>
#include <stdio.h>

PSServer::PSServer(CLoop *loop) : CStringServer(loop) {}
PSServer::~PSServer() {}

bool PSServer::HandleRequest(uv_stream_t *stream, const char *req, const char *buf, int size)
{
	if (_stricmp(req, "valid") == 0)
		return OnValid(stream, buf, size);
	else if (_stricmp(req, "nicks") == 0)
		return OnSendNicks(stream, buf, size);
	else if (_stricmp(req, "check") == 0)
		return OnCheckNicks(stream, buf, size);
	else if (_stricmp(req, "search") == 0)
		return OnSearchUsers(stream, buf, size);
	else if (_stricmp(req, "others") == 0)
		return OnReverseBuddies(stream, buf, size);
	else if (_stricmp(req, "otherslist") == 0)
		return OnOthersList(stream, buf, size);
	else if (_stricmp(req, "uniquesearch") == 0)
		return OnUniqueSearch(stream, buf, size);
	else if (_stricmp(req, "profilelist") == 0)
		return OnProfileList(stream, buf, size);
	else if (_stricmp(req, "pmatch") == 0)
		return OnProductMatching(stream, buf, size);

	printf("[PlayerSearch] Unknown request: %s\n", req);
	return false;
}

// Unused
bool PSServer::OnNewConnection(uv_stream_t*) { return true; }

bool PSServer::OnValid(uv_stream_t *client, const char *buf, int)
{
	char buffer[256];
	char email[EMAIL_MAX_LEN];
	CDBResult *result = new CDBResult();

	buffer[0] = email[0] = 0;

	if (!get_gs_data(buf, "email", email, sizeof(email)))
	{
		delete result;
		return false;
	}

	_snprintf_s(buffer, sizeof(buffer), "SELECT COUNT(id) FROM `users` WHERE `email` = '%s'", email);

	if (!RunDBQueryWithResult(buffer, result))
	{
		delete result;
		return false;
	}

	if (atoi(result->GetColumnByRow(0, 0).c_str()) < 1)
		Write(client, "\\vr\\0\\final\\");
	else
		Write(client, "\\vr\\1\\final\\");

	delete result;
	return true;
}

bool PSServer::OnSendNicks(uv_stream_t *stream, const char *buf, int size)
{
	puts(buf);
	return false;
}

bool PSServer::OnCheckNicks(uv_stream_t *stream, const char *buf, int size)
{
	puts(buf);
	return false;
}

bool PSServer::OnSearchUsers(uv_stream_t *stream, const char *buf, int size)
{
	puts(buf);
	return false;
}

bool PSServer::OnReverseBuddies(uv_stream_t *stream, const char *buf, int size)
{
	puts(buf);
	return false;
}

bool PSServer::OnOthersList(uv_stream_t *stream, const char *buf, int size)
{
	puts(buf);
	return false;
}

bool PSServer::OnUniqueSearch(uv_stream_t *stream, const char *buf, int size)
{
	puts(buf);
	return false;
}

bool PSServer::OnProfileList(uv_stream_t *stream, const char *buf, int size)
{
	puts(buf);
	return false;
}

bool PSServer::OnProductMatching(uv_stream_t *stream, const char *buf, int size)
{
	puts(buf);
	return false;
}

