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

#if CPP_CONNECTOR
#include <driver/mysql_connection.h>
#include <cppconn/exception.h>
#endif

// Internal SQL Connection
#if CPP_CONNECTOR
sql::Connection *_ptr_sqlconnection_3h8hgfq28hfg = NULL;
#else
MYSQL *_ptr_sqlconnection_3h8hgfq28hfg = NULL;
#endif

DLLAPI std::string EscapeSQLString(std::string str)
{
#if CPP_CONNECTOR
	if (!_ptr_sqlconnection_3h8hgfq28hfg)
		return "";
	sql::mysql::MySQL_Connection *msqc = dynamic_cast<sql::mysql::MySQL_Connection*>(_ptr_sqlconnection_3h8hgfq28hfg);
	return msqc->escapeString(str);
#else
	char *x = (char*)malloc(sizeof(char)*(str.length()*2+5));
	if (!x)
		return "";
	mysql_real_escape_string(_ptr_sqlconnection_3h8hgfq28hfg, x, str.c_str(), str.length());
	std::string k = std::string(x);
	free(x);
	return k;
#endif
}

DLLAPI void EscapeSQLString(std::string &str)
{
	str = EscapeSQLString(str.c_str());
}

#if CPP_CONNECTOR
DLLAPI void SetConnectionPtr(sql::Connection *con)
#else
DLLAPI void SetConnectionPtr(MYSQL *con)
#endif
{
	_ptr_sqlconnection_3h8hgfq28hfg = con;
}

DLLAPI bool RunDBQuery(std::string str)
{
#if CPP_CONNECTOR
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
#else
	if (mysql_query(_ptr_sqlconnection_3h8hgfq28hfg, str.c_str()) != 0)
	{
		printf("[Database] Cannot execute query. Error: %s\n", mysql_error(_ptr_sqlconnection_3h8hgfq28hfg));
		return false;		
	}
	
	return true;
#endif
}

DLLAPI bool RunDBQuery(std::string query, sql::ResultSet **rs)
{
#if CPP_CONNECTOR
	sql::Statement* stmt = _ptr_sqlconnection_3h8hgfq28hfg->createStatement();
#endif
	sql::ResultSet *res = NULL;

#if CPP_CONNECTOR
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
#else
	*rs = new sql::ResultSet();
	if (!(*rs)->executeQuery(_ptr_sqlconnection_3h8hgfq28hfg, query))
	{
		delete rs;

		printf("[Database] Cannot execute query. Error: %s\n",  mysql_error(_ptr_sqlconnection_3h8hgfq28hfg));
		return false;		
	}
#endif

	return true;
}