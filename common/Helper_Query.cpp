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
	query[0] = 0;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "UPDATE profiles SET sesskey=NULL WHERE profileid='%u'", profileid);

	RunDBQuery(query);
}

DLLAPI int GetPublicMaskFromProfileID(unsigned int pid)
{
	CDBResult *res = new CDBResult();
	char query[61];
	query[0] = 0;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "SELECT publicmask FROM profiles WHERE profileid=%u", pid);

	if (!RunDBQueryWithResult(query, res))
	{
		delete res;
		return -1;
	}


	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", res->GetColumnByRow(0, 0).c_str());
	return atoi(query);
}

DLLAPI bool GetProfileInfo(unsigned int pid, GPIInfoCache *out, unsigned int *id_out)
{
	CDBResult *res = new CDBResult();
	char query[1024];
	std::vector<std::string> vec;

	query[0] = 0;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "SELECT profiles.uniquenick, profiles.nick, profiles.firstname, profiles.lastname, profiles.latitude," //5
		" profiles.longitude, profiles.publicmask, profiles.userid, profiles.aim, profiles.picture," //10
		" profiles.occupationid, incomeid, profiles.industryid, profiles.marriedid, profiles.childcount," //15
		" profiles.interests1, profiles.ownership1, profiles.connectiontype, profiles.sex, profiles.zipcode," //20
		" profiles.countrycode, profiles.homepage, profiles.birthday, profiles.birthmonth, profiles.birthyear," // 25
		" profiles.location, profiles.icq FROM profiles WHERE profileid=%u", pid);

	if (!RunDBQueryWithResult(query, res))
	{
		delete res;
		return false;
	}

	vec = res->GetRow(0);
	delete res;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%u", vec.at(7).c_str());
	*id_out = atoi(query);

	strncpy_s(out->nick, sizeof(out->nick), vec.at(1).c_str(), sizeof(out->nick) - 1);
	strncpy_s(out->uniquenick, sizeof(out->uniquenick), vec.at(0).c_str(), sizeof(out->uniquenick) - 1);
	strncpy_s(out->firstname, sizeof(out->firstname), vec.at(2).c_str(), sizeof(out->firstname) - 1);
	strncpy_s(out->lastname, sizeof(out->lastname), vec.at(3).c_str(), sizeof(out->lastname) - 1);
	strncpy_s(out->homepage, sizeof(out->homepage), vec.at(22).c_str(), sizeof(out->homepage) - 1);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(27).c_str());
	out->icquin = atoi(query);

	strncpy_s(out->zipcode, sizeof(out->zipcode), vec.at(20).c_str(), sizeof(out->zipcode) - 1);
	strncpy_s(out->countrycode, sizeof(out->countrycode), vec.at(21).c_str(), sizeof(out->countrycode) - 1);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%f", vec.at(5).c_str());
	out->longitude = (float)atof(query);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%f", vec.at(4).c_str());
	out->latitude = (float)atof(query);

	strncpy_s(out->place, sizeof(out->place), vec.at(26).c_str(), sizeof(out->place) - 1);
	
	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(23).c_str());
	out->birthday = atoi(query);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(24).c_str());
	out->birthmonth = atoi(query);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(25).c_str());
	out->birthyear = atoi(query);

	if (strcmp(vec.at(19).c_str(), "MALE") == 0)
		out->sex = GP_MALE;
	else if (strcmp(vec.at(19).c_str(), "FEMALE") == 0)
		out->sex = GP_FEMALE;
	else
		out->sex = GP_PAT;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(6).c_str());
	out->publicmask = atoi(query);

	strncpy_s(out->aimname, sizeof(out->aimname), vec.at(9).c_str(), sizeof(out->aimname) - 1);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(10).c_str());
	out->pic = atoi(query);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(11).c_str());
	out->occupationid = atoi(query);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(13).c_str());
	out->industryid = atoi(query);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(12).c_str());
	out->incomeid = atoi(query);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(14).c_str());
	out->marriedid = atoi(query);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(15).c_str());
	out->childcount = atoi(query);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(16).c_str());
	out->interests1 = atoi(query);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(17).c_str());
	out->ownership1 = atoi(query);

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "%d", vec.at(18).c_str());
	out->conntypeid = atoi(query);

	return true;
}
