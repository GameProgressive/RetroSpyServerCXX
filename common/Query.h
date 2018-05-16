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

#include <mysql.h>
#include <vector>

class ResultSet
{
public:
DLLAPI ResultSet();
DLLAPI ~ResultSet();

DLLAPI bool executeQuery(MYSQL* conn, std::string str);

DLLAPI bool first();
DLLAPI unsigned int getUInt(size_t index);
DLLAPI std::string getString(size_t index);
DLLAPI int getInt(size_t index);
DLLAPI double getDouble(size_t index);

DLLAPI size_t getRows();

DLLAPI bool next();

private:
	std::vector<std::vector<std::string>> m_rows;
	size_t m_pos;
};

/**
	Function: RunDBQuery
	Description: Executes a generic query
	Parameters:
		str => the query
	Return: true if the query was successfully executed, otherwise false
*/
DLLAPI bool RunDBQuery(MYSQL* mysql, std::string str);

/**
	Function: RunDBQuery
	Description: Executes a generic query and store it's result to the passed result res
	Parameters:
		str => the query
		rs => the result set to be filled
	Return: true if the query was successfully executed, otherwise false
*/
DLLAPI bool RunDBQuery(MYSQL *mysql, std::string str, ResultSet **rs);

/**
	Function: EscapeSQLString
	Description: Add escapes from a standard string
	Parameters:
		str => The string to be escaped
	Return: Escaped string
*/
DLLAPI std::string EscapeSQLString(MYSQL* con, std::string str);

/**
	Function: EscapeSQLString
	Description: Modify the passed string by adding escapes
	Parameters:
		str => The string to be escaped
*/
DLLAPI void EscapeSQLString(MYSQL* con, std::string &str);
#endif
