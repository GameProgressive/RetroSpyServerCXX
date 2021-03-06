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

#include <MDK/Query.h>

#include <stdio.h>
#include <string.h>
#include <limits.h>

bool ExecuteFirstQuery(CDatabase* db, CResultSet *res, std::string query)
{
	if (!res->ExecuteQuery(db, query))
		return false;

	if (!res->GotoFirstRow())
		return false;

	return true;
}

bool GetProfileIDFromNickEmail(CDatabase* db, const char *nick, const char *email, unsigned int* pid)
{
	std::string query = "";
	std::string _email = email, _nick = nick;
	CResultSet *res = NULL;

	query = "SELECT profiles.profileid FROM profiles INNER JOIN"
			" users ON profiles.userid=users.userid WHERE"
			" users.email='";
	if (!mdk_escape_query_string(db, _email))
		return false;
	query += _email;
	query += "' AND profiles.nick='";
	if (!mdk_escape_query_string(db, _nick))
		return false;
	query += _nick;
	query += "'";

	res = new CResultSet();
	if (!ExecuteFirstQuery(db, res, query))
	{
		delete res;
		return false;
	}

	*pid = res->GetUIntFromRow(0);

	delete res;
	return true;
}

bool GetProfileIDFromNickEmailAndPass(CDatabase* c, const char *nick, const char *email, const char* pass, unsigned int* id)
{
	std::string query = "";
	std::string _email = email, _nick = nick, _pass = pass;
	CResultSet* res = NULL;
	
	query = "SELECT profiles.profileid FROM profiles INNER JOIN"
	" users ON profiles.userid=users.userid WHERE"
	" users.email='";
	
	if (!mdk_escape_query_string(c, _email))
		return false;
	
	query += _email;
	query += "' AND profiles.nick='";
	
	if (!mdk_escape_query_string(c, _nick))
		return false;
	
	query += _nick;
	query += "' AND users.password='";
	
	if (!mdk_escape_query_string(c, _pass))
		return false;
	
	query += "'";
	
	res = new CResultSet();

	if (!ExecuteFirstQuery(c, res, query))
	{
		delete res;
		return false;
	}

	*id = res->GetUIntFromRow(0);

	delete res;
	return true;
}

bool GetUserIDFromEmail(CDatabase* db, const char* email, unsigned int* id)
{
	std::string _email = email, query = "";
	CResultSet* res = NULL;
	
	if (!mdk_escape_query_string(db, _email))
		return false;
	
	query = "SELECT userid FROM users WHERE email='";
	query += _email;
	query += "'";
	
	res = new CResultSet();
	if (!ExecuteFirstQuery(db, res, query))
	{
		delete res;
		return false;
	}

	*id = res->GetUIntFromRow(0);

	delete res;
	return true;
}

void GetUniqueNickFromProfileID(CDatabase* db, unsigned int pid, char *unick, int size)
{
	std::string query = "";
	CResultSet *res = new CResultSet();

	query = "SELECT uniquenick FROM profiles WHERE profileid='";
	query += std::to_string(pid);
	query += "'";

	if (!ExecuteFirstQuery(db, res, query))
	{
		delete res;
		return;
	}

	strncpy(unick, res->GetStringFromRow(0).c_str(), size);
	unick[size - 1] = '\0';
	
	delete res;
}

bool GetProfileIDFromUniqueNick(CDatabase* db, const char *unick, unsigned int* out)
{
	std::string query = "", _unick = unick;
	CResultSet *res = new CResultSet();

	query = "SELECT profileid FROM profiles WHERE uniquenick='";
	if (!mdk_escape_query_string(db, _unick))
	{
		delete res;
		return false;
	}
	query += _unick;
	query += "'";

	if (!ExecuteFirstQuery(db, res, query))
	{
		delete res;
		return false;
	}

	*out = res->GetUIntFromRow(0);

	delete res;
	return true;
}

void GetPasswordFromUserID(CDatabase* db, char *out, int out_len, unsigned int id)
{
	CResultSet *res = new CResultSet();
	char query[61];
	query[0] = 0;

	snprintf(query, sizeof(query), "SELECT password FROM users WHERE userid='%u'", id);

	if (!ExecuteFirstQuery(db, res, query))
	{
		delete res;
		return;
	}

	strncpy(out, res->GetStringFromRow(0).c_str(), out_len);
	out[out_len - 1] = '\0';
	
	delete res;
}

unsigned int GetUserIDFromProfileID(CDatabase* db, unsigned int id, unsigned int* out)
{
	CResultSet *res = new CResultSet();
	char query[61];
	unsigned int ret = 0;
	query[0] = 0;

	snprintf(query, sizeof(query), "SELECT userid FROM profiles WHERE profileid='%u'", id);

	if (!ExecuteFirstQuery(db, res, query))
	{
		delete res;
		return false;
	}

	ret = res->GetUIntFromRow(0);
	*out = ret;

	delete res;
	return true;
}

int AssignSessionKeyFromProfileID(CDatabase* db, unsigned int profileid)
{
	int ssk = 0;
	bool bx = true;
	char query[61];
	query[0] = 0;

	while (bx)
	{
		ssk = RandInt(1, INT_MAX);
		snprintf(query, sizeof(query), "UPDATE profiles SET sesskey=%d WHERE profileid='%u'", ssk, profileid);

		if (mdk_only_run_query(db, query))
			bx = false;
	}

	return ssk;
}

void FreeSessionKey(CDatabase* db, unsigned int profileid)
{
	char query[61];
	query[0] = 0;

	snprintf(query, sizeof(query), "UPDATE profiles SET sesskey=NULL WHERE profileid='%u'", profileid);

	mdk_only_run_query(db, query);
}

int GetPublicMaskFromProfileID(CDatabase* db, unsigned int pid)
{
	CResultSet *res = new CResultSet();
	std::string query = "";
	int ret = 0;

	query = "SELECT publicmask FROM profiles WHERE profileid=";
	query += std::to_string(pid);

	if (!ExecuteFirstQuery(db, res, query))
	{
		delete res;
		return ret;
	}

	ret = res->GetIntFromRow(0);

	delete res;
	return ret;
}

bool GetProfileInfo(CDatabase* db, unsigned int pid, GPIInfoCache *out, unsigned int *id_out)
{
#define resget(x) res->GetStringFromRow(x).c_str()

	CResultSet *res = new CResultSet();
	std::string query = "";

	query = "SELECT profiles.uniquenick, profiles.nick, profiles.firstname, profiles.lastname, profiles.latitude," //5
		" profiles.longitude, profiles.publicmask, profiles.userid, profiles.aim, profiles.picture," //9
		" profiles.occupationid, profiles.incomeid, profiles.industryid, profiles.marriedid, profiles.childcount," //14
		" profiles.interests1, profiles.ownership1, profiles.connectiontype, profiles.sex, profiles.zipcode," //19
		" profiles.countrycode, profiles.homepage, profiles.birthday, profiles.birthmonth, profiles.birthyear," //24
		" profiles.location, profiles.icq, users.email FROM profiles INNER JOIN users ON users.userid=profiles.userid WHERE profiles.profileid=";
	
	query += std::to_string(pid);

	if (!ExecuteFirstQuery(db, res, query))
	{
		delete res;
		return false;
	}

	strncpy(out->uniquenick, resget(0), sizeof(out->uniquenick));
	out->uniquenick[sizeof(out->uniquenick) - 1] = '\0';
	strncpy(out->nick, resget(1), sizeof(out->nick));
	out->nick[sizeof(out->nick) - 1] = '\0';
	strncpy(out->firstname, resget(2), sizeof(out->firstname));
	out->firstname[sizeof(out->firstname) - 1] = '\0';
	strncpy(out->lastname, resget(3), sizeof(out->lastname));
	out->lastname[sizeof(out->lastname) - 1] = '\0';
	out->latitude = (float)res->GetDoubleFromRow(4);
	out->longitude = (float)res->GetDoubleFromRow(5);
	out->publicmask = res->GetIntFromRow(6);
	*id_out = res->GetUIntFromRow(7);
	strncpy(out->aimname, resget(8), sizeof(out->aimname));
	out->aimname[sizeof(out->aimname) - 1] = '\0';
	out->pic = res->GetIntFromRow(9);
	out->occupationid = res->GetIntFromRow(10);
	out->incomeid = res->GetIntFromRow(11);
	out->industryid = res->GetIntFromRow(12);
	out->marriedid = res->GetIntFromRow(13);
	out->childcount = res->GetIntFromRow(14);
	out->interests1 = res->GetIntFromRow(15);
	out->ownership1 = res->GetIntFromRow(16);
	out->conntypeid = res->GetIntFromRow(17);

	if (res->GetStringFromRow(18).compare("MALE") == 0)
		out->sex = GP_MALE;
	else if (res->GetStringFromRow(18).compare("FEMALE") == 0)
		out->sex = GP_FEMALE;
	else
		out->sex = GP_PAT;	

	strncpy(out->zipcode, resget(19), sizeof(out->zipcode));
	out->zipcode[sizeof(out->zipcode) - 1] = '\0';
	strncpy(out->countrycode, resget(20), sizeof(out->countrycode));
	out->countrycode[sizeof(out->countrycode) - 1] = '\0';
	strncpy(out->homepage, resget(21), sizeof(out->homepage));
	out->homepage[sizeof(out->homepage) - 1] = '\0';
	out->birthday = res->GetIntFromRow(22);
	out->birthmonth = res->GetIntFromRow(23);
	out->birthyear = res->GetIntFromRow(24);
	strncpy(out->place, resget(25), sizeof(out->place));
	out->place[sizeof(out->place) - 1] = '\0';
	out->icquin = res->GetIntFromRow(26);
	strncpy(out->email, resget(27), sizeof(out->email));
	out->email[sizeof(out->email) - 1] = '\0';

	delete res;

	return true;
}

bool GetProfileIDFromAuthToken(CDatabase* db, const char *authtoken, unsigned int* out)
{
	std::string query = "";
	std::string _authtoken = authtoken;
	CResultSet *res = NULL;
	
	if (!mdk_escape_query_string(db, _authtoken))
		return false;
	
	res = new CResultSet();

	query = "SELECT profileid FROM authtoken WHERE token='";
	query += _authtoken;
	query += "'";

	if (!ExecuteFirstQuery(db, res, query))
	{
		delete res;
		return false;
	}

	*out = res->GetUIntFromRow(0);

	delete res;
	return true;
}

int RegisterUser(CDatabase* db, const char* email, const char* nick, const char* pass, unsigned int* userid)
{
	std::string query = "", _email = email, _nick = nick, _pass = pass;
	CResultSet* res = NULL;
	
	if (!mdk_escape_query_string(db, _email))
		return false;
	
	if (!mdk_escape_query_string(db, _nick))
		return false;
	
	if (!mdk_escape_query_string(db, _pass))
		return false;
	
	query = "SELECT uniquenick FROM profiles WHERE uniquenick='";
	query += _nick;
	query += "'";
	
	res = new CResultSet();
	
	if (!res->ExecuteQuery(db, query.c_str()))
	{
		delete res;
		return GP_NEWUSER;
	}
	
	if (res->GetTotalRows() > 0)
	{
		delete res;
		return GP_NEWUSER_UNIQUENICK_INUSE;
	}
	
	query = "SELECT email FROM users WHERE email='";
	query += _email;
	query += "'";
	
	if (!res->ExecuteQuery(db, query.c_str()))
	{
		delete res;
		return GP_NEWUSER;
	}
	
	if (res->GetTotalRows() > 0)
	{
		delete res;
		return GP_NEWUSER;
	}
	
	query = "INSERT INTO users(email, password, deleted, emailverified) VALUES ('";
	query += _email;
	query += "', '";
	query += _pass;
	query += "', 0, 1)";
	
	if (!mdk_only_run_query(db, query))
	{
		delete res;
		return GP_NEWUSER;		
	}
	
	query = "SELECT userid FROM users WHERE email='";
	query += _email;
	query += "'";

	if (!ExecuteFirstQuery(db, res, query))
	{
		delete res;
		return GP_NEWUSER;
	}
	
	query = "INSERT INTO profiles(userid, uniquenick, nick) VALUES (";
	query += std::to_string(res->GetUIntFromRow(0));
	query += ", '";
	query += _nick;
	query += "', '";
	query += _nick;
	query += "')";
	
	if (!mdk_only_run_query(db, query))
	{
		delete res;
		return GP_NEWUSER;
	}
	
	query = "SELECT profileid FROM profiles WHERE uniquenick='";
	query += _nick;
	query += "'";
	
	if (!ExecuteFirstQuery(db, res, query))
	{
		delete res;
		return GP_NEWUSER;		
	}
	
	*userid = res->GetUIntFromRow(0);

	delete res;
	return -1;
}