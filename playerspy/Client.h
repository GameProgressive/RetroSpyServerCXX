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
#ifndef PY_CLIENT_H
#define PY_CLIENT_H

#include "Types.h"

#include <Defines.h>

#include <uv.h>
#include <time.h>

#include <list>
#include <string>

class CClient
{
public:
	CClient(uv_stream_t *stream, unsigned int vid);
	~CClient();

	unsigned int GetUserID();
	unsigned int GetProfileID();

	time_t GetLastPacket();

	bool HasBuddy(CClient *c);
	bool HasBuddy(unsigned int id);

	bool HasBlocked(CClient *c);
	bool HasBlocked(unsigned int id);

	void SendBuddyStatus(CClient *c);

	int GetPartnerID();

	bool ProductInviteable(unsigned int pid);

	bool Handle(const char *req, const char *buf, int len);

	short GetPort();
	int GetIP();
	
	int GetSDKVersion();

	GPEnum GetStatusType();
	const char *GetStatus();

	GPEnum GetQuietFlags();

	const char *GetLocation();

	void Disconnect();

	// Operator overload
	bool operator==(CClient& c);
	bool operator!=(CClient& c);

	unsigned int m_vectorid;

private:
	char m_email[GP_EMAIL_LEN];

	unsigned int m_profileid;
	unsigned int m_userid;

	unsigned int m_sesskey;

	uv_stream_t* m_stream;

	int m_sdkversion;

	int m_partnerid;

	short m_port;
	int m_ip;

	time_t m_lastpacket;

	GPEnum m_status_type;
	char m_status[GP_STATUS_STRING_LEN];

	GPEnum m_quiet_flags;

	char m_location[GP_STATUS_STRING_LEN];


	std::list<unsigned int> m_blocked;
	std::list<unsigned int> m_buddies;
	std::list<unsigned int> m_products;
	
private:
	bool HandleLogin(const char *buf, int len);
	bool HandleInviteTo(const char *buf, int len);
	bool HandleNewUser(const char *buf, int len);

	void SendBuddyInfo(unsigned int id);

	void Write(const char *str);
	void Write(std::string str);
};

#endif
