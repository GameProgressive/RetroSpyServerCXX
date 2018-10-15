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

#include <MDK/Database.h>
#include <MDK/TemplateSocket.h>

#include "Types.h"

#include <Defines.h>

#include <time.h>

#include <list>
#include <string>

class CClient
{
public:
	CClient(mdk_socket stream, size_t vector_id, CDatabase* db);
	~CClient();

	bool HasBuddy(CClient *c);
	bool HasBuddy(unsigned int id);

	bool HasBlocked(CClient *c);
	bool HasBlocked(unsigned int id);

	void SendBuddyStatus(CClient *c);

	bool ProductInviteable(unsigned int pid);

	bool Handle(const char *req, const char *buf, int len);

	inline unsigned int GetUserID() { return m_userid; }
	inline unsigned int GetProfileID() { return m_profileid; }

	inline time_t GetLastPacket() { return m_lastpacket; }

	inline int GetPartnerID() { return m_partnerid; }

	inline short GetPort() { return m_port; }
	inline int GetIP() { return m_ip; }
	
	inline int GetSDKVersion() { return m_sdkversion; }

	inline GPEnum GetStatusType() { return m_status_type; }
	inline const char *GetStatus() { return m_status; }

	inline GPEnum GetQuietFlags() { return m_quiet_flags; }

	inline const char *GetLocation() { return m_location; }

	inline mdk_socket GetSocket() { return m_stream; }

	inline unsigned int GetVectorID() { return m_vector_id; }

	void Disconnect();

	// Operator overload
	bool operator==(CClient& c);
	bool operator!=(CClient& c);

private:
	char m_email[GP_EMAIL_LEN];

	unsigned int m_profileid;
	unsigned int m_userid;

	unsigned int m_sesskey;

	size_t		m_vector_id;

	mdk_socket m_stream;

	int m_sdkversion;

	int m_partnerid;

	short m_port;
	int m_ip;

	time_t m_lastpacket;

	GPEnum m_status_type;
	char m_status[GP_STATUS_STRING_LEN];

	GPEnum m_quiet_flags;

	char m_location[GP_STATUS_STRING_LEN];

	bool m_SentBuddies;
	bool m_SentAddRequests;
	bool m_bLogged;

	std::list<unsigned int> m_blocked;
	std::list<unsigned int> m_buddies;
	std::list<unsigned int> m_products;
	
private:
	bool HandleLogin(const char *buf, int len);
	bool HandleInviteTo(const char *buf, int len);
	bool HandleNewUser(const char *buf, int len);
	bool HandleStatus(const char *buf, int len);
	bool HandleGetProfile(const char *buf, int len);

	void SendBuddyInfo(unsigned int id);
	void SendBuddies();
	void SendAddRequests();

	void LoadBuddies();
	void LoadBlockedList();

	void SendMessages();

	void Write(const char *str);
	void Write(std::string str);
	
	CDatabase* m_dbConnect;
};

#endif
