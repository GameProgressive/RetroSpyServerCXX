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
#ifndef RETROSPYCOMMON_DATABASE_H
#define RETROSPYCOMMON_DATABASE_H

#include "Defines.h"

#include <string>

#if CPP_CONNECTOR
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/connection.h>
#else
	
#include <mysql.h>
#include <vector>

namespace sql
{
class ResultSet
{
public:
ResultSet();
~ResultSet();

bool executeQuery(MYSQL* conn, std::string str);

bool first();
unsigned int getUInt(size_t index);
std::string getString(size_t index);
int getInt(size_t index);
double getDouble(size_t index);

private:
	std::vector<std::vector<std::string>> m_rows;
};
}
#endif

/**
	Function: RunDBQuery
	Description: Executes a generic query
	Parameters:
		str => the query
	Return: true if the query was successfully executed, otherwise false
*/
DLLAPI bool RunDBQuery(std::string str);

/**
	Function: RunDBQuery
	Description: Executes a generic query and store it's result to the passed result res
	Parameters:
		str => the query
		rs => the result set to be filled
	Return: true if the query was successfully executed, otherwise false
*/
DLLAPI bool RunDBQuery(std::string str, sql::ResultSet **rs);

/**
	Function: EscapeSQLString
	Description: Add escapes from a standard string
	Parameters:
		str => The string to be escaped
	Return: Escaped string
*/
DLLAPI std::string EscapeSQLString(std::string str);

/**
	Function: EscapeSQLString
	Description: Modify the passed string by adding escapes
	Parameters:
		str => The string to be escaped
*/
DLLAPI void EscapeSQLString(std::string &str);

/**
	Function: SetConnectionPtr
	Description: Set the internal pointer of an SQL connection
		NOTE: Please set the connection pointer BEFORE calling any Query API
	Parameters:
		c => The pointer of the connection
*/
#if CPP_CONNECTOR
DLLAPI void SetConnectionPtr(sql::Connection *c);
#else
DLLAPI void SetConnectionPtr(MYSQL *con);
#endif

#endif
