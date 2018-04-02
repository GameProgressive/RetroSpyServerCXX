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

#include <Defines.h>

#include <uv.h>

#include <list>

class CClient
{
public:
	CClient(uv_stream_t *stream);
	~CClient();

	int GetUserID();
	int GetProfileID();

	time_t GetLastPacket();

	bool HasBuddy(CClient *c);
	bool HasBuddy(int id);

	bool HasBlocked(CClient *c);
	bool HasBlocked(int id);

	void SendBuddyStatus(CClient *c);

	size_t GetVectorID();

	bool ProductInviteable(int pid);

	bool Handle(const char *req, const char *buf, int len);

	// Operator overload
	bool operator==(CClient& c)
	{
		if ( (GetProfileID() == c.GetProfileID()) && (GetUserID() == c.GetUserID()) )
			return true;

		return false;
	}

	bool operator!=(CClient& c)
	{
		if ( (GetProfileID() != c.GetProfileID()) || (GetUserID() != c.GetUserID()) )
			return true;

		return false;
	}
private:
	char m_email[GP_EMAIL_LEN];

	char m_challenge[11];

	int m_profileid;
	int m_userid;

	int m_sesskey;

	uv_stream_t* m_stream;

	int m_sdkversion;

	int m_port;

	time_t m_lastpacket;

	std::list<int> m_blocked;
	std::list<int> m_buddies;
	std::list<int> m_products;
	
private:
	bool HandleLogin(const char *buf, int len);
	bool HandleInviteTo(const char *buf, int len);
	bool HandleNewUser(const char *buf, int len);

	void SendBuddyInfo(int id);
};

#endif
