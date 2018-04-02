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

/* Fixed define of mysql */
#ifndef _MSC_STDINT_H_
	#define _MSC_STDINT_H_
#endif
#include <mysql.h>

#include <vector>
#include <string>

class CDBResult
{
public:
	DLLAPI CDBResult();
	DLLAPI ~CDBResult();

	bool DLLAPI Execute(MYSQL_RES *res);

	my_ulonglong DLLAPI GetTotalRows();
	my_ulonglong DLLAPI GetProcessedRows();

	my_ulonglong DLLAPI GetFieldsCount();

	std::vector<std::string> DLLAPI GetRow(size_t index);
	DLLAPI std::string GetColumnByRow(size_t row, size_t column);

private:
	std::vector<std::vector<std::string>> m_rows;
	
	std::vector<std::string> _Dummy;

	my_ulonglong m_totalrows;
	my_ulonglong m_fields;
	my_ulonglong m_processrow;

};

/*
	Function: RunDBQuery
	Description: Run a query
	Return: true if the query successfully executed, otherwise false
	Parameters:
		stmt => The query
*/
DLLAPI bool RunDBQuery(const char *stmt);

/*
	Function: RunDBQuery
	Description: Run a query and store it's result
	Return: true if the query successfully executed, otherwise false
	Parameters:
		stmt => The query
		out => The result to store the query

	NOTE: CREATE THE INSTANCE OF THE RESULT BEFORE EXECUTING THIS
*/
DLLAPI bool RunDBQueryWithResult(const char *stmt, CDBResult *out);

/*
	Function: SetDBInstance
	Description: Set the Database instance that will be used to process query
	Parameters:
		db => A pointer to the database instance

	NOTE: Call this once inside the Master Server (Don't call it into submodules unless you know
		what you're doing)

*/
DLLAPI void SetDBInstance(MYSQL *db);

/*
	Function: EscapeSQLString
	Description: Add escapes from a standard string
	Parameters:
		from => The string to start
		to => The string to produce
		length => Length of the string
	Return: Length of the encoded string or -1 if an error occurs
*/
DLLAPI unsigned long EscapeSQLString(const char *from, char *to, unsigned long length);

#endif

