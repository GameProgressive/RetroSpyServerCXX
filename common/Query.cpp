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
#include "Query.h"

#include <stdio.h>

#include <driver/mysql_connection.h>
#include <cppconn/exception.h>

// Internal SQL Connection
sql::Connection *_ptr_sqlconnection_3h8hgfq28hfg = NULL;

DLLAPI std::string EscapeSQLString(std::string str)
{
	if (!_ptr_sqlconnection_3h8hgfq28hfg)
		return "";
	sql::mysql::MySQL_Connection *msqc = dynamic_cast<sql::mysql::MySQL_Connection*>(_ptr_sqlconnection_3h8hgfq28hfg);
	return msqc->escapeString(str);
}

DLLAPI void EscapeSQLString(std::string &str)
{
	str = EscapeSQLString(str.c_str());
}

DLLAPI void SetConnectionPtr(sql::Connection *con)
{
	_ptr_sqlconnection_3h8hgfq28hfg = con;
}

DLLAPI bool RunDBQuery(std::string str)
{
	sql::Statement* stmt = _ptr_sqlconnection_3h8hgfq28hfg->createStatement();
	sql::ResultSet *res = NULL;

	if (!stmt)
		return false;

	try
	{
		stmt->executeQuery(str);
	}
	catch (sql::SQLException &ex)
	{
		delete stmt;

		printf("[Database] Cannot execute query. Error: %s\n", ex.what());
		return false;
	}

	delete stmt;
	return true;
}

DLLAPI bool RunDBQuery(std::string query, sql::ResultSet **rs)
{
	sql::Statement* stmt = _ptr_sqlconnection_3h8hgfq28hfg->createStatement();
	sql::ResultSet *res = NULL;

	if (!stmt)
		return false;

	try
	{
		*rs = stmt->executeQuery(query);
	}
	catch (sql::SQLException &ex)
	{
		delete stmt;

		printf("[Database] Cannot execute query. Error: %s\n", ex.what());
		return false;
	}

	delete stmt;

	if (!*rs)
		return false;

	return true;
}