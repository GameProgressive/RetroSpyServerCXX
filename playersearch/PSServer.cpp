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

#include <MDK/Utility.h>
#include <MDK/Query.h>

#include <Helper.h>

#include <string.h>
#include <stdio.h>

PSServer::PSServer(CDatabase* db)
{
	m_dbConnection = db;
}
PSServer::~PSServer() {}

bool PSServer::HandleRequest(mdk_socket stream, const char *req, const char *buf, int size)
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
bool PSServer::OnNewConnection(mdk_socket, int) { return true; }

bool PSServer::OnValid(mdk_socket client, const char *buf, int)
{
	std::string buffer="";
	std::string email = "";
	CResultSet *result = NULL;

	if (!get_gs_data(buf, email, "email"))
	{
		return false;
	}

	result = new CResultSet();

	buffer = "SELECT COUNT(userid) FROM `users` WHERE `email` = '";
	if (!mdk_escape_query_string(m_dbConnection, email))
	{
		delete result;
		return false;
	}
	buffer += email;
	buffer += "'";

	if (!result->ExecuteQuery(m_dbConnection, buffer))
	{
		delete result;
		return false;
	}

	if (!result->GotoFirstRow())
		Write(client, "\\vr\\0\\final\\");

	if (result->GetIntFromRow(0) < 1)
		Write(client, "\\vr\\0\\final\\");
	else
		Write(client, "\\vr\\1\\final\\");

	delete result;
	return true;
}

bool PSServer::OnSendNicks(mdk_socket stream, const char *buf, int)
{
	std::string email = "", pass = "", gamename = "", str = "";
	bool bSendUnique = false;
	size_t i = 0;
	CResultSet *result = NULL;

	// Get data from buffer

	if (!get_gs_data(buf, email, "email"))
		return false;

	if (get_gs_data(buf, pass, "passenc"))
	{
		// Uncrypt the password
		gs_pass_decode(pass);
	}
	else
	{
		if (!get_gs_data(buf, pass, "pass"))
			return false;
	}

	if (get_gs_data(buf, gamename, "gamename"))
		bSendUnique = true;

	// Create the query and execute it
	str = "SELECT profiles.nick, profiles.uniquenick FROM profiles INNER "
		"JOIN users ON profiles.userid=users.userid WHERE users.email='";
	if (!mdk_escape_query_string(m_dbConnection, email))
		return false;

	str += email;
	str += "' AND password='";
	if (!mdk_escape_query_string(m_dbConnection, pass))
		return false;

	str += pass;
	str += "'";

	result = new CResultSet();

	if (!result->ExecuteQuery(m_dbConnection, str))
	{
		delete result;
		
		Write(stream, "\\nr\\\\ndone\\final\\");
		return false;
	}
	
	if (!result->GotoFirstRow())
	{
		delete result;
		
		Write(stream, "\\nr\\\\ndone\\final\\");
		return false;
		
	}

	str = "\\nr\\" + std::to_string(result->GetTotalRows());

	// Get all the nicks and store them
	do
	{
		str += "\\nick\\";
		str += result->GetStringFromRow(0);

		if (bSendUnique)
		{
			str += "\\uniquenick\\";
			str += result->GetStringFromRow(1);
		}
	} while(result->GotoNextRow());

	str += "\\ndone\\final\\";

	// Send to the socket
	Write(stream, str);

	delete result;

	return true;
}

bool PSServer::OnCheckNicks(mdk_socket, const char *buf, int)
{
	puts(buf);
	return false;
}

bool PSServer::OnSearchUsers(mdk_socket, const char *buf, int)
{
	puts(buf);
	return false;
}

bool PSServer::OnReverseBuddies(mdk_socket socket, const char *buf, int)
{
	/*
		GP_MASK_EMAIL       = 0x00000020,
	*/
	CResultSet* rs = NULL;
	std::string query = "";
	unsigned int profileid = 0;

	if (!get_gs_data(buf, query, "profileid"))
		return false;

	profileid = (unsigned int)std::stol(query);

	query = "SELECT profiles.profileid, nick, uniquenick, firstname, lastname, users.email, publicmask FROM profiles INNER JOIN friends ON profiles.profileid = friends.profileid INNER JOIN users ON users.userid = profiles.userid WHERE friends.targetid = ";
	query += std::to_string(profileid);

	rs = new CResultSet();

	if (!rs->ExecuteQuery(m_dbConnection, query))
	{
		delete rs;
		return false;
	}

	if (!rs->GotoFirstRow())
	{
		Write(socket, "\\others\\odone\\final\\");
		return true;
	}

	query = "\\others";

	do
	{
		query += "\\o\\";
		query += rs->GetUIntFromRow(0);
		query += "\\nick\\";
		query += rs->GetStringFromRow(1);
		query += "\\uniquenick\\";
		query += rs->GetStringFromRow(2);
		query += "\\first\\";
		query += rs->GetStringFromRow(3);
		query += "\\last\\";
		query += rs->GetStringFromRow(4);
		query += "\\email\\";

		if (rs->GetIntFromRow(6) & GP_MASK_EMAIL)
		{
			query += rs->GetStringFromRow(5);
		}
	} while (rs->GotoNextRow());

	query += "\\odone\\final\\";

	Write(socket, query);

	return true;
}

bool PSServer::OnOthersList(mdk_socket, const char *buf, int)
{
	puts(buf);
	return false;
}

bool PSServer::OnUniqueSearch(mdk_socket stream, const char *buf, int)
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

bool PSServer::OnProfileList(mdk_socket, const char *buf, int)
{
	puts(buf);
	return false;
}

bool PSServer::OnProductMatching(mdk_socket, const char *buf, int)
{
	puts(buf);
	return false;
}

