/*
    This file is part of RetroSpy Server.

    RetroSpy Server is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RetroSpy Server is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSEc  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with RetroSpy Server.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "PSServer.h"

#include "../common/RSString.h"
#include "../common/Query.h"
#include "../common/Helper.h"

#include <string.h>
#include <stdio.h>

PSServer::PSServer(CLoop *loop, MYSQL*con) : CStringServer(loop)
{
	m_con = con;
}
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

	LOG_ERROR("PlayerSearch", "Unknown request \"%s\" received!", req);
	return false;
}

// Unused
bool PSServer::OnNewConnection(uv_stream_t*) { return true; }

bool PSServer::OnValid(uv_stream_t *client, const char *buf, int)
{
	std::string buffer="";
	char email[GP_EMAIL_LEN];
	ResultSet *result = NULL;

	email[0] = 0;

	if (!get_gs_data(buf, "email", email, sizeof(email)))
	{
		return false;
	}

	buffer = "SELECT COUNT(userid) FROM `users` WHERE `email` = '";
	buffer += EscapeSQLString(m_con, email);
	buffer += "'";

	if (!RunDBQuery(m_con, buffer, &result))
	{
		delete result;
		return false;
	}

	if (result->first())
		Write(client, "\\vr\\0\\final\\");
	else
		Write(client, "\\vr\\1\\final\\");

	delete result;
	return true;
}

bool PSServer::OnSendNicks(uv_stream_t *stream, const char *buf, int)
{
	char email[GP_EMAIL_LEN];
	char pass[GP_PASSWORD_LEN];
	char gamename[GS_GAMENAME_LEN];
	char passenc[GP_PASSWORDENC_LEN];

	std::string str = "";

	bool bSendUnique = false;
	
	size_t i = 0;

	ResultSet *result = NULL;

	gamename[0] = pass[0] = email[0] = passenc[0] = 0;

	// Get data from buffer

	if (!get_gs_data(buf, "email", email, sizeof(email)))
		return false;

	if (get_gs_data(buf, "passenc", passenc, sizeof(passenc)))
	{
		// Uncrypt the password
		gs_pass_decode(passenc, pass);
	}
	else
	{
		if (!get_gs_data(buf, "pass", pass, sizeof(pass)))
			return false;
	}

	if (get_gs_data(buf, "gamename", gamename, sizeof(gamename)))
		bSendUnique = true;

	// Create the query and execute it
	str = "SELECT profiles.nick, profiles.uniquenick FROM profiles INNER "
		"JOIN users ON profiles.userid=users.userid WHERE users.email='";
	str += EscapeSQLString(m_con, email);
	str += "' AND password='";
	str += EscapeSQLString(m_con, pass);
	str += "'";

	if (!RunDBQuery(m_con, str, &result))
	{
		delete result;
		
		Write(stream, "\\nr\\\\ndone\\final\\");
		return false;
	}
	
	if (!result->first())
	{
		delete result;
		
		Write(stream, "\\nr\\\\ndone\\final\\");
		return false;
		
	}

	// We use gamename to avoid big memory usage
	_snprintf_s(gamename, sizeof(gamename), sizeof(gamename) - 1, "\\nr\\%d", result->getRows());

	str = gamename;

	// Get all the nicks and store them
	do
	{
		str += "\\nick\\";
		str += result->getString(0);

		if (bSendUnique)
		{
			str += "\\uniquenick\\";
			str += result->getString(1);
		}
	} while(result->next());

	str += "\\ndone\\final\\";

	// Send to the socket
	Write(stream, str);

	delete result;

	return true;
}

bool PSServer::OnCheckNicks(uv_stream_t *, const char *buf, int)
{
	puts(buf);
	return false;
}

bool PSServer::OnSearchUsers(uv_stream_t *, const char *buf, int)
{
	puts(buf);
	return false;
}

bool PSServer::OnReverseBuddies(uv_stream_t *, const char *buf, int)
{
	puts(buf);
	return false;
}

bool PSServer::OnOthersList(uv_stream_t *, const char *buf, int)
{
	puts(buf);
	return false;
}

bool PSServer::OnUniqueSearch(uv_stream_t *stream, const char *buf, int)
{
	char preferrednick[GP_NICK_LEN];
	std::string sendmsg = "\\us\\7";

	preferrednick[0] = 0;

	if (!get_gs_data(buf, "preferrednick", preferrednick, sizeof(preferrednick)))
		return false;

	// TODO: Change this to something more real

	gs_make_valid(preferrednick);

	// 7 times (us=7)
	sendmsg += "\\nick\\";
	sendmsg += preferrednick;
	sendmsg += "\\nick\\";
	sendmsg += preferrednick;
	sendmsg += "\\nick\\";
	sendmsg += preferrednick;
	sendmsg += "\\nick\\";
	sendmsg += preferrednick;
	sendmsg += "\\nick\\";
	sendmsg += preferrednick;
	sendmsg += "\\nick\\";
	sendmsg += preferrednick;
	sendmsg += "\\nick\\";
	sendmsg += preferrednick;

	sendmsg += "\\usdone\\final\\";

	Write(stream, sendmsg);

	return true;
}

bool PSServer::OnProfileList(uv_stream_t *, const char *buf, int)
{
	puts(buf);
	return false;
}

bool PSServer::OnProductMatching(uv_stream_t *, const char *buf, int)
{
	puts(buf);
	return false;
}

