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
#include "Client.h"

#include "ClientManager.h"
#include "PYServer.h"

#include "../common/Server.h"
#include "../common/RSString.h"
#include "../common/Helper.h"
#include "../common/chc_endian.h"

CClient::CClient(uv_stream_t *stream, unsigned int vid)
{
	m_stream = stream;
	m_vectorid = vid;

	m_email[0] = m_location[0] = 0;
	m_profileid = m_userid = m_sesskey = m_partnerid = 0;
	m_sdkversion = 0;
	m_port = 0;
	m_status_type = m_quiet_flags = GP_ERROR;

	m_ip = CServer::GetIPFromStream(stream);

	strncpy_s(m_status, sizeof(m_status), "Offline", sizeof(m_status)-1);
}

CClient::~CClient()
{
	Disconnect();
}

void CClient::Disconnect()
{
	if (m_profileid != 0)
		FreeSessionKey(m_profileid);

	m_profileid = 0;
}

unsigned int CClient::GetUserID()
{
	return m_userid;
}

unsigned int CClient::GetProfileID()
{
	return m_profileid;
}

short CClient::GetPort()
{
	return m_port;
}
 
int CClient::GetSDKVersion()
{
	return m_sdkversion;
}

int CClient::GetPartnerID()
{
	return m_partnerid;
}

int CClient::GetIP()
{
	return m_ip;
}

time_t CClient::GetLastPacket()
{
	return m_lastpacket;
}

GPEnum CClient::GetQuietFlags()
{
	return m_quiet_flags;
}

GPEnum CClient::GetStatusType()
{
	return m_status_type;
}

const char *CClient::GetStatus()
{
	return m_status;
}

const char *CClient::GetLocation()
{
	return m_location;
}

bool CClient::HasBuddy(CClient *c)
{
	return HasBuddy(c->GetProfileID());
}

bool CClient::HasBuddy(unsigned int id)
{
	std::list<unsigned int>::iterator iterator = m_buddies.begin();
	unsigned int pid = 0;

	while (iterator != m_buddies.end())
	{
		pid = *iterator;
		if (id == pid)
			return true;

		iterator++;
	}

	return false;
}

bool CClient::HasBlocked(CClient *c)
{
	return HasBlocked(c->GetProfileID());
}

bool CClient::HasBlocked(unsigned int id)
{
	std::list<unsigned int>::iterator iterator = m_blocked.begin();
	unsigned int pid = 0;

	while (iterator != m_blocked.end())
	{
		pid = *iterator;

		if (pid == id)
			return true;

		iterator++;
	}

	return false;
}

void CClient::SendBuddyStatus(CClient *c)
{
	SendBuddyInfo(c->GetProfileID());
}

bool CClient::ProductInviteable(unsigned int id)
{
	std::list<unsigned int>::iterator iterator = m_products.begin();
	unsigned int pid = 0;

	while (iterator != m_products.end())
	{
		pid = *iterator;

		if (pid == id)
			return true;

		iterator++;
	}

	return false;
}

bool CClient::Handle(const char *req, const char *buf, int len)
{
	m_lastpacket = time(NULL);

	if (strcmp(req, "login") == 0)
		return HandleLogin(buf, len);
	else if (strcmp(req, "newuser") == 0)
		return HandleNewUser(buf, len);
	else if (strcmp(req, "inviteto") == 0)
		return HandleInviteTo(buf, len);

	if (m_userid == 0)
	{
		return false;
	}

	if (strcmp(req, "logout") == 0)
	{
		Disconnect();
		return false;
	}
	else
		puts(req);

	return true;
}

bool CClient::HandleLogin(const char *buf, int)
{
	char unick[GP_UNIQUENICK_LEN];
	char user[GP_EMAIL_LEN + GP_NICK_LEN + 2];
	char password[GP_PASSWORD_LEN];
	char sdkver[10];
	char response[33];
	char clientchall[GP_CLIENTCHALL_LEN];
	char authtoken[GP_AUTHTOKEN_LEN];
	char lt[GP_LOGIN_TICKET_LEN]; //TODO: Understand login ticket
	char sendbuf[1025];
	char proof[MD5_BUFFER_LEN];

	response[0] = clientchall[0] = sdkver[0] = unick[0] = user[0] = 0;
	lt[0] = authtoken[0] = sendbuf[0] = password[0] = 0;

	// Get data from buffer
	if (!get_gs_data(buf, "challenge", clientchall, sizeof(clientchall)))
		return false;

	get_gs_data(buf, "uniquenick", unick, sizeof(unick));
	get_gs_data(buf, "user", user, sizeof(user));

	get_gs_data(buf, "sdkversion", sdkver, sizeof(sdkver));
	if (sdkver[0] != 0)
		m_sdkversion = atoi(sdkver);

	get_gs_data(buf, "port", sdkver, sizeof(sdkver));
	if (sdkver[0] != 0)
		m_port = (short)atoi(sdkver);

	get_gs_data(buf, "partnerid", sdkver, sizeof(sdkver));
	if (sdkver[0] != 0)
		m_partnerid = atoi(sdkver);

	if ( (user[0] == 0) && (unick[0] == 0) )
	{
		// TODO: Handle authtoken login
		puts(buf);
		return false;
	}

	if (authtoken[0] == 0)
	{
		// Handle partner id proof
		if (m_partnerid != GS_PARTNER_ID)
		{
			if (unick[0] != 0)
				_snprintf_s(authtoken, sizeof(authtoken), sizeof(authtoken) - 1, "%d@%s", m_partnerid, unick);
			else
				_snprintf_s(authtoken, sizeof(authtoken), sizeof(authtoken) - 1, "%d@%s", m_partnerid, user);
		}
		else
		{
			if (unick[0] != 0)
				_snprintf_s(authtoken, sizeof(authtoken), sizeof(authtoken) - 1, "%s", unick);
			else
				_snprintf_s(authtoken, sizeof(authtoken), sizeof(authtoken) - 1, "%s", user);
		}
	}

	if (unick[0] == 0)
	{
		// Login with user (nick@email)

		char nick[GP_NICK_LEN], email[GP_EMAIL_LEN];

		if (!user_to_emailnick(user, email, sizeof(email), nick, sizeof(nick)))
			return false;


		m_profileid = GetProfileIDFromNickEmail(nick, email);

		if (m_profileid == 0)
			return false;

		GetUniqueNickFromProfileID(m_profileid, unick, sizeof(unick));
	}
	else if (user[0] == 0)
	{
		// Login with uniquenick

		m_profileid = GetProfileIDFromUniqueNick(unick);

		if (m_profileid == 0)
			return false;
	}

	// Assign the remaining data key
	m_userid = GetUserIDFromProfileID(m_profileid);
	m_sesskey = AssignSessionKeyFromProfileID(m_profileid);
	GetPasswordFromUserID(password, sizeof(password), m_userid);

	// Set login ticket
	strrnd(lt, sizeof(lt), "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ][");
	lt[GP_LOGIN_TICKET_LEN - 1] = '_';
	lt[GP_LOGIN_TICKET_LEN - 2] = '_';

	// Do proof
	gs_do_proof(proof, password, authtoken, PYServer::GetServerChallenge(), clientchall);

	_snprintf_s(sendbuf, sizeof(sendbuf), sizeof(sendbuf) - 1,
		"\\lc\\2\\sesskey\\%d\\proof\\%s\\userid\\%u\\profileid\\%u"
		"\\uniquenick\\%s\\lt\\%s\\id\\%d\\final\\",  m_sesskey, proof,
		m_userid, m_profileid, unick, lt, PYServer::GetServerID());

	Write(sendbuf);

	return true;
}

bool CClient::HandleInviteTo(const char *buf, int)
{
	puts(buf);
	return false;
}

bool CClient::HandleNewUser(const char *buf, int)
{
	puts(buf);
	return false;
}

void CClient::SendBuddyInfo(unsigned int id)
{
	CClient *c = CClientManager::GetFromProfileID(id);
	char statstr[GP_STATUS_STRING_LEN + GP_STATUS_STRING_LEN + 128 + 60];

	if (c == NULL || c->HasBlocked(m_profileid))
		_snprintf_s(statstr, sizeof(statstr), sizeof(statstr)-1, "\\bm\\%d\\f\\%u\\msg\\|s|0|ss|Offline\\final\\", GPI_BM_STATUS, m_profileid);
	else
	{
		_snprintf_s(statstr, sizeof(statstr), sizeof(statstr)-1, "\\bm\\%d\\f\\%u\\|s|%s|ss|%s%s%s|ip|%d|p|%d|gm|%d\\final\\", 
			GPI_BM_STATUS, m_profileid, c->GetStatusType(), c->GetStatus(), c->GetLocation()[0] != 0 ? "|ls|" : "", c->GetLocation(), 
			reverse_endian32(c->GetIP()), reverse_endian16(c->GetPort()), c->GetQuietFlags());
	}

	Write(statstr);
}


// Operators
bool CClient::operator==(CClient& c)
{
	if ( (GetProfileID() == c.GetProfileID()) && (GetUserID() == c.GetUserID()) )
		return true;

	return false;
}

bool CClient::operator!=(CClient& c)
{
	if ( (GetProfileID() != c.GetProfileID()) || (GetUserID() != c.GetUserID()) )
		return true;

	return false;
}

void CClient::Write(std::string str)
{
	CServer::Write(m_stream, str);
}

void CClient::Write(const char *str)
{
	CServer::Write(m_stream, str);
}
