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
#include "Helper.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>

bool ExecuteFirstQuery(mdk_mysql* c, ResultSet **res, std::string q)
{
	if (!Database::RunDBQuery(c, q, res))
		return false;

	if ((*res)->first())
		return false;

	return true;
}

unsigned int GetProfileIDFromNickEmail(mdk_mysql* c, const char *nick, const char *email)
{
	std::string query = "";
	std::string _email = "", _nick = "";
	ResultSet *res = NULL;
	unsigned int ret = 0;

	query = "SELECT profiles.profileid FROM profiles INNER JOIN"
			" users ON profiles.userid=users.userid WHERE"
			" users.email='";
	query += Database::EscapeSQLString(c, email);
	query += "' AND profiles.nick='";
	query += Database::EscapeSQLString(c, nick);
	query += "'";

	if (!ExecuteFirstQuery(c, &res, query))
	{
		delete res;
		return ret;
	}

	ret = res->getUInt(0);

	delete res;
	return ret;
}

void GetUniqueNickFromProfileID(mdk_mysql* c, unsigned int pid, char *unick, int size)
{
	std::string query = "";
	ResultSet *res = NULL;

	query = "SELECT uniquenick FROM profiles WHERE profileid='";
	query += std::to_string(pid);
	query += "'";

	if (!ExecuteFirstQuery(c, &res, query))
	{
		delete res;
		return;
	}

	strncpy_s(unick, size, res->getString(0).c_str(), size - 1);

	delete res;
}

unsigned int GetProfileIDFromUniqueNick(mdk_mysql* c, const char *unick)
{
	std::string query = "";
	ResultSet *res = NULL;
	unsigned int ret = 0;

	query = "SELECT profileid FROM profiles WHERE uniquenick='";
	query += Database::EscapeSQLString(c, unick);
	query += "'";

	if (!ExecuteFirstQuery(c, &res, query))
	{
		delete res;
		return ret;
	}

	ret = res->getUInt(0);

	delete res;
	return ret;
}

void GetPasswordFromUserID(mdk_mysql* c, char *out, int out_len, unsigned int id)
{
	ResultSet *res = NULL;
	char query[61];
	query[0] = 0;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "SELECT password FROM users WHERE userid='%u'", id);

	if (!ExecuteFirstQuery(c, &res, query))
	{
		delete res;
		return;
	}

	strncpy_s(out, out_len, res->getString(0).c_str(), out_len - 1);

	delete res;
}

unsigned int GetUserIDFromProfileID(mdk_mysql* c, unsigned int id)
{
	ResultSet *res = NULL;
	char query[61];
	unsigned int ret = 0;
	query[0] = 0;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "SELECT userid FROM profiles WHERE profileid='%u'", id);

	if (!ExecuteFirstQuery(c, &res, query))
	{
		delete res;
		return ret;
	}

	ret = res->getUInt(0);
	delete res;
	return (unsigned)std::stoi(query);
}

int AssignSessionKeyFromProfileID(mdk_mysql* c, unsigned int profileid)
{
	int ssk = 0;
	bool bx = true;
	char query[61];
	query[0] = 0;

	while (bx)
	{
		ssk = RandInt(1, INT_MAX);
		_snprintf_s(query, sizeof(query), sizeof(query) - 1, "UPDATE profiles SET sesskey=%d WHERE profileid='%u'", ssk, profileid);

		if (Database::RunDBQuery(c, query))
			bx = false;
	}

	return ssk;
}

void FreeSessionKey(mdk_mysql* c, unsigned int profileid)
{
	char query[61];
	query[0] = 0;

	_snprintf_s(query, sizeof(query), sizeof(query) - 1, "UPDATE profiles SET sesskey=NULL WHERE profileid='%u'", profileid);

	Database::RunDBQuery(c, query);
}

int GetPublicMaskFromProfileID(mdk_mysql* c, unsigned int pid)
{
	ResultSet *res = NULL;
	std::string query = "";
	int ret = 0;

	query = "SELECT publicmask FROM profiles WHERE profileid=";
	query += std::to_string(pid);

	if (!ExecuteFirstQuery(c, &res, query))
	{
		delete res;
		return ret;
	}

	ret = res->getInt(0);

	delete res;
	return ret;
}

bool GetProfileInfo(mdk_mysql* c, unsigned int pid, GPIInfoCache *out, unsigned int *id_out)
{
#define resget(x) res->getString(x).c_str()

	ResultSet *res = NULL;
	std::string query = "";

	query = "SELECT profiles.uniquenick, profiles.nick, profiles.firstname, profiles.lastname, profiles.latitude," //5
		" profiles.longitude, profiles.publicmask, profiles.userid, profiles.aim, profiles.picture," //9
		" profiles.occupationid, profiles.incomeid, profiles.industryid, profiles.marriedid, profiles.childcount," //14
		" profiles.interests1, profiles.ownership1, profiles.connectiontype, profiles.sex, profiles.zipcode," //19
		" profiles.countrycode, profiles.homepage, profiles.birthday, profiles.birthmonth, profiles.birthyear," //24
		" profiles.location, profiles.icq, users.email FROM profiles INNER JOIN users ON users.userid=profiles.userid WHERE profiles.profileid=";
	
	query += std::to_string(pid);

	if (!ExecuteFirstQuery(c, &res, query))
	{
		delete res;
		return false;
	}

	strncpy_s(out->uniquenick, sizeof(out->uniquenick), resget(0), sizeof(out->uniquenick) - 1);
	strncpy_s(out->nick, sizeof(out->nick), resget(1), sizeof(out->nick) - 1);
	strncpy_s(out->firstname, sizeof(out->firstname), resget(2), sizeof(out->firstname) - 1);
	strncpy_s(out->lastname, sizeof(out->lastname), resget(3), sizeof(out->lastname) - 1);
	out->latitude = (float)res->getDouble(4);
	out->longitude = (float)res->getDouble(5);
	out->publicmask = res->getInt(6);
	*id_out = res->getUInt(7);
	strncpy_s(out->aimname, sizeof(out->aimname), resget(8), sizeof(out->aimname) - 1);
	out->pic = res->getInt(9);
	out->occupationid = res->getInt(10);
	out->incomeid = res->getInt(11);
	out->industryid = res->getInt(12);
	out->marriedid = res->getInt(13);
	out->childcount = res->getInt(14);
	out->interests1 = res->getInt(15);
	out->ownership1 = res->getInt(16);
	out->conntypeid = res->getInt(17);

	if (res->getString(18).compare("MALE") == 0)
		out->sex = GP_MALE;
	else if (res->getString(18).compare("FEMALE") == 0)
		out->sex = GP_FEMALE;
	else
		out->sex = GP_PAT;	

	strncpy_s(out->zipcode, sizeof(out->zipcode), resget(19), sizeof(out->zipcode) - 1);
	strncpy_s(out->countrycode, sizeof(out->countrycode), resget(20), sizeof(out->countrycode) - 1);
	strncpy_s(out->homepage, sizeof(out->homepage), resget(21), sizeof(out->homepage) - 1);
	out->birthday = res->getInt(22);
	out->birthmonth = res->getInt(23);
	out->birthyear = res->getInt(24);
	strncpy_s(out->place, sizeof(out->place), resget(25), sizeof(out->place) - 1);
	out->icquin = res->getInt(26);
	strncpy_s(out->email, sizeof(out->email), resget(27), sizeof(out->email) - 1);

	delete res;

	return true;
}
