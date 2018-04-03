/*
    This file is part of RetroSpy Server.

    RetroSpy Server is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
#define RSC_EXPORT 1 //Export the methods
#include "Helper.h"

#include "Query.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>

DLLAPI unsigned int GetProfileIDFromNickEmail(const char *nick, const char *email)
{
	char query[251];
	char _email[GP_EMAIL_LEN];
	char _nick[GP_NICK_LEN];
	CDBResult *res = new CDBResult();
	query[0] = _email[0] = _nick[0] = 0;

	// SQL injections fix
	EscapeSQLString(nick, _nick, strlen(nick));
	EscapeSQLString(email, _email, strlen(email));

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "SELECT profiles.profileid FROM profiles INNER JOIN"
			" users ON profiles.userid=users.userid WHERE"
			" users.email='%s' AND profiles.nick='%s'", _email, _nick);

	if (!RunDBQueryWithResult(query, res))
	{
		delete res;
		return 0;
	}

	strncpy_s(query, sizeof(query), res->GetColumnByRow(0, 0).c_str(), sizeof(query) - 1);

	delete res;
	return atoi(query);
}

DLLAPI void GetUniqueNickFromProfileID(unsigned int pid, char *unick, int size)
{
	char query[121];
	CDBResult *res = new CDBResult();
	query[0] = 0;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "SELECT uniquenick FROM profiles WHERE profileid='%u'", pid);

	if (!RunDBQueryWithResult(query, res))
	{
		delete res;
		return;
	}

	strncpy_s(unick, size, res->GetColumnByRow(0, 0).c_str(), size - 1);

	delete res;
}

DLLAPI unsigned int GetProfileIDFromUniqueNick(const char *unick)
{
	char query[121];
	char _unick[GP_UNIQUENICK_LEN];
	CDBResult *res = new CDBResult();
	query[0] = _unick[0] = 0;

	EscapeSQLString(unick, _unick, strlen(unick));

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "SELECT profileid FROM profiles WHERE uniquenick='%s'", _unick);

	if (!RunDBQueryWithResult(query, res))
	{
		delete res;
		return 0;
	}

	strncpy_s(query, sizeof(query), res->GetColumnByRow(0, 0).c_str(), sizeof(query) - 1);

	delete res;
	return atoi(query);
}

DLLAPI void GetPasswordFromUserID(char *out, int out_len, unsigned int id)
{
	char query[61];
	CDBResult *res = new CDBResult();
	query[0] = 0;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "SELECT password FROM users WHERE userid='%u'", id);

	if (!RunDBQueryWithResult(query, res))
	{
		delete res;
		return;
	}

	strncpy_s(out, out_len, res->GetColumnByRow(0, 0).c_str(), out_len - 1);

	delete res;
}

DLLAPI unsigned int GetUserIDFromProfileID(unsigned int id)
{
	char query[61];
	CDBResult *res = new CDBResult();
	query[0] = 0;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "SELECT userid FROM profiles WHERE profileid='%u'", id);

	if (!RunDBQueryWithResult(query, res))
	{
		delete res;
		return 0;
	}

	strncpy_s(query, sizeof(query), res->GetColumnByRow(0, 0).c_str(), sizeof(query) - 1);

	delete res;
	return atoi(query);
}

DLLAPI int AssignSessionKeyFromProfileID(unsigned int profileid)
{
	int ssk = 0;
	bool bx = true;
	char query[61];
	query[0] = 0;

	while (bx)
	{
		ssk = Util_RandInt(1, INT_MAX);
		_snprintf_s(query, sizeof(query), sizeof(query) - 1, "UPDATE profiles SET sesskey=%d WHERE profileid='%u'", ssk, profileid);

		if (RunDBQuery(query))
			bx = false;
	}

	return ssk;
}

DLLAPI void FreeSessionKey(unsigned int profileid)
{
	char query[61];
	query[0];

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "UPDATE profiles SET sesskey=NULL WHERE profileid='%u'", profileid);

	RunDBQuery(query);
}
