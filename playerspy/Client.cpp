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
#include "../common/Query.h"
#include "../common/chc_endian.h"

#include <string>

CClient::CClient(uv_stream_t *stream, unsigned int vid, MYSQL* con)
{
	m_stream = stream;
	m_vectorid = vid;
	m_con = con;

	m_email[0] = m_location[0] = 0;
	m_profileid = m_userid = m_sesskey = m_partnerid = 0;
	m_sdkversion = 0;
	m_port = 0;
	m_status_type = m_quiet_flags = GP_ERROR;
	m_SentBuddies = m_SentAddRequests = false;

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
		FreeSessionKey(m_con, m_profileid);

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
	else if (strcmp(req, "status") == 0)
		return HandleStatus(buf, len);
	else if (strcmp(req, "getprofile") == 0)
		return HandleGetProfile(buf, len);
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


		m_profileid = GetProfileIDFromNickEmail(m_con, nick, email);

		if (m_profileid == 0)
			return false;

		GetUniqueNickFromProfileID(m_con, m_profileid, unick, sizeof(unick));
	}
	else if (user[0] == 0)
	{
		// Login with uniquenick

		m_profileid = GetProfileIDFromUniqueNick(m_con,  unick);

		if (m_profileid == 0)
			return false;
	}

	// Assign the remaining data key
	m_userid = GetUserIDFromProfileID(m_con, m_profileid);
	m_sesskey = AssignSessionKeyFromProfileID(m_con, m_profileid);
	GetPasswordFromUserID(m_con, password, sizeof(password), m_userid);

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

	LoadBuddies();
	LoadBlockedList();
	SendMessages();

	if (m_sdkversion & GPI_NEW_LIST_RETRIEVAL_ON_LOGIN)
		SendBuddies();

	return true;
}

bool CClient::HandleInviteTo(const char *buf, int)
{
	char products[256];
	int pdo = 0;
	char *pch = NULL, *ctx = NULL;

	if (!get_gs_data(buf, "products", products, sizeof(products)))
		return false;

	m_products.clear();

	if (strcmp(products, "") == 0)
		return true;

	if (strchr(products, '\\') != NULL)
	{
		pch = strtok_s(products, ",", &ctx);

		while (pch != NULL)
		{
			pdo = atoi(pch);
			m_products.push_front(pdo);
			//pch = strtok(pch, ",");
			pch = strtok_s(NULL, ",", &ctx); // TODO: Finish this
		}
	}
	else
	{
		pdo = atoi(products);
		m_products.push_front(pdo);
	}

	return true;
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
		_snprintf_s(statstr, sizeof(statstr), sizeof(statstr)-1, "\\bm\\%d\\f\\%u\\|s|%d|ss|%s%s%s|ip|%d|p|%d|gm|%d\\final\\", 
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

bool CClient::HandleStatus(const char *buf, int)
{
	get_gs_data(buf, "statstring", m_status, sizeof(m_status));
	get_gs_data(buf, "locstring", m_location, sizeof(m_location));

	if (!m_SentBuddies)
		SendBuddies();

	if (!m_SentAddRequests)
		SendAddRequests();

	CClientManager::SendUpdateStatus(this);
	return true;
}

void CClient::SendBuddies()
{
	std::list<unsigned int>::iterator it;
	unsigned int pid = 0;

	m_SentBuddies = true;

	if (m_sdkversion & GPI_NEW_LIST_RETRIEVAL_ON_LOGIN)
	{
		std::string str = "";
		char txp[16];

		_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%u", m_buddies.size());

		it = m_buddies.begin();

		str = "\\bdy\\";
		str += txp;
		str += "\\list\\";

		while (it != m_buddies.end())
		{
			pid = *it;

			_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%u", pid);

			str += txp;

			it++;

			if (it != m_buddies.end())
			{
				str += ",";	
			}
		}

		Write(str);
	}

	it = m_buddies.begin();

	while (it != m_buddies.end())
	{
		pid = *it;
		SendBuddyInfo(pid);
		it++;
	}
}

void CClient::SendAddRequests()
{
	char query[256];
	ResultSet *res = NULL;
	my_ulonglong x = 0;
	std::string str = "";

	query[0] = 0;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "SELECT `profileid`, `syncrequested`, `reason` FROM `addrequests` WHERE `targetid`=%u", m_profileid);
	if (!RunDBQuery(m_con, query, &res))
	{
		delete res;
		return;
	}

	m_SentAddRequests = true;

	do
	{
		str = "\\bm\\";
		_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", GPI_BM_REQUEST);
		str += query;
		str += "\\f\\";
		str += res->getString(0);
		str += "\\msg\\";
		str += res->getString(1);
		str += "|signed|";
		str += res->getString(2);
		str += "\\final\\";

		Write(str);
	} while (res->next());

	delete res;
}

void CClient::LoadBuddies()
{
	char query[128];
	my_ulonglong x = 0;
	ResultSet *res = NULL;

	query[0] = 0;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, 
		"SELECT `targetid` FROM `friends` WHERE `profileid`=%u", m_profileid);

	if (!RunDBQuery(m_con, query, &res))
	{
		delete res;
		return;
	}
	
	if (!res->first())
	{
		delete res;
		return;
	}

	m_buddies.clear();

	do
	{
		m_buddies.push_front(res->getInt(0));
	} while (res->next());
	
	delete res;
}

void CClient::LoadBlockedList()
{
	char query[128];
	my_ulonglong x = 0;
	ResultSet *res = NULL;

	query[0] = 0;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, 
		"SELECT `targetid` FROM `blocked` WHERE `profileid`=%u", m_profileid);

	if (!RunDBQuery(m_con, query, &res))
	{
		delete res;
		return;
	}
	
	if (!res->first())
	{
		delete res;
		return;
	}

	m_blocked.clear();

	do
	{
		m_blocked.push_front(res->getInt(0));
	} while (res->next());

	delete res;
}

void CClient::SendMessages()
{
	char query[256];
	ResultSet *res = NULL;
	my_ulonglong x = 0;
	std::string str = "";

	query[0] = 0;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "SELECT `message`, `from`, Unix_Timestamp(`date`) FROM `messages` WHERE `to`=%u", m_profileid);

	if (!RunDBQuery(m_con, query, &res))
	{
		delete res;
		return;
	}

	if (!res->first())
	{
		delete res;
		return;
	}
	
	do
	{
		_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", GPI_BM_MESSAGE); 
		
		str = "\\bm\\";
		str += query;
		str += "\\f\\";
		str += res->getString(1);
		str += "\\date\\";
		str += res->getString(2);
		str += "\\msg\\";
		str += res->getString(0);
		str += "\\final\\";
	} while (res->next());

	delete res;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "DELETE FROM `messages` WHERE `to`=%u", m_profileid);
	RunDBQuery(m_con, query);
}

bool CClient::HandleGetProfile(const char *buf, int)
{
	std::string str = "\\pi\\profileid\\";
	char txp[101];
	char sign[MD5_BUFFER_LEN];
	int id = 0;
	unsigned int pid = 0;
	unsigned int uid = 0;
	GPIInfoCache info;

	if (!m_SentBuddies)
		SendBuddies();

	if (!get_gs_data(buf, "id", txp, sizeof(txp)))
		return false;

	id = atoi(txp);

	if (!get_gs_data(buf, "profileid", txp, sizeof(txp)))
		return false;
	
	str += txp;
	pid = atoi(txp);

	str += "\\userid\\";

	if (!GetProfileInfo(m_con, pid, &info, &uid))
		return false;

	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%u", uid);
	str += txp;

	str += "\\nick\\";
	str += info.nick;
	str += "\\uniquenick\\";
	str += info.uniquenick;
	
	if (info.publicmask & GP_MASK_EMAIL)
	{
		str += "\\email\\";
		str += info.email;
	}

	str += "\\firstname\\";
	str += info.firstname;
	str += "\\lastname\\";
	str += info.lastname;

	if (info.publicmask & GP_MASK_COUNTRYCODE)
	{
		str += "\\countrycode\\";
		str += info.countrycode;
	}

	str += "\\aim\\";
	str += info.aimname;

	str += "\\pmask\\";
	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%d", info.publicmask);
	str += txp;
	
	str += "\\pic\\";
	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%d", info.pic);
	str += txp;

	str += "\\ooc\\";
	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%d", info.occupationid);
	str += txp;

	str += "\\ind\\";
	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%d", info.industryid);
	str += txp;

	str += "\\inc\\";
	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%d", info.incomeid);
	str += txp;

	str += "\\mar\\";
	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%d", info.marriedid);
	str += txp;

	str += "\\chc\\";
	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%d", info.childcount);
	str += txp;

	str += "\\i1\\";
	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%d", info.interests1);
	str += txp;

	str += "\\o1\\";
	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%d", info.ownership1);
	str += txp;

	str += "\\conn\\";
	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%d", info.conntypeid);
	str += txp;

	if (info.publicmask & GP_MASK_SEX)
	{
		str += "\\sex\\";
		_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%d", info.sex);
		str += txp;
	}

	if (info.publicmask & GP_MASK_ZIPCODE)
	{
		str += "\\zipcode\\";
		str += info.zipcode;
	}

	if (info.publicmask & GP_MASK_HOMEPAGE)
	{
		str += "\\homepage\\";
		str += info.homepage;
	}

	if (info.publicmask & GP_MASK_BIRTHDAY)
	{
		int bdd = 0;

		bdd |= (info.birthday << 24);
		bdd |= (info.birthmonth << 16);
		bdd |= info.birthyear;

		str += "\\birthday\\";
		_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%d", bdd);
		str += txp;
	}

	str += "\\lon\\";
	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%f", info.longitude);
	str += txp;

	str += "\\lat\\";
	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%f", info.latitude);
	str += txp;

	str += "\\loc\\";
	str += info.place;

	hash_md5(str.c_str(), str.length(), sign);

	str += "\\sig\\";
	str += sign;

	str += "\\id\\";
	_snprintf_s(txp, sizeof(txp), sizeof(txp) - 1, "%d", id);
	str += txp;

	// Old support
	str += "\\mp\\4\\st\\";
	
	str += "\\final\\";

	Write(str);
	return true;
}
