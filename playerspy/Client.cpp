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

CClient::CClient(uv_stream_t *stream)
{
	m_stream = stream;
	m_challenge[0] = m_email[0] = 0;
	m_profileid = m_userid = m_sesskey = m_sdkversion = m_port = 0;
}

CClient::~CClient()
{
}

int CClient::GetUserID()
{
	return m_userid;
}

int CClient::GetProfileID()
{
	return m_profileid;
}

time_t CClient::GetLastPacket()
{
	return m_lastpacket;
}

bool CClient::HasBuddy(CClient *c)
{
	return HasBuddy(c->GetProfileID());
}

bool CClient::HasBuddy(int id)
{
	std::list<int>::iterator iterator = m_buddies.begin();
	int pid = 0;

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

bool CClient::HasBlocked(int id)
{
	std::list<int>::iterator iterator = m_blocked.begin();
	int pid = 0;

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

bool CClient::ProductInviteable(int id)
{
	std::list<int>::iterator iterator = m_products.begin();
	int pid = 0;

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
		return false;
	else
		puts(req);

	return true;
}

bool CClient::HandleLogin(const char *buf, int len)
{
	puts(buf);
	return false;
}

bool CClient::HandleInviteTo(const char *buf, int len)
{
	puts(buf);
	return false;
}

bool CClient::HandleNewUser(const char *buf, int len)
{
	puts(buf);
	return false;
}

void CClient::SendBuddyInfo(int id)
{

}