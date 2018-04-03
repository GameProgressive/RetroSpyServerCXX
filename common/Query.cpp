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

MYSQL* _QueryDatabaseInstanceMysql = NULL;

DLLAPI void SetDBInstance(MYSQL *db)
{
	_QueryDatabaseInstanceMysql = db;
}

DLLAPI bool RunDBQuery(const char *stmt)
{
	if (mysql_query(_QueryDatabaseInstanceMysql, stmt) != 0)
	{
		printf("[Query] Error: %s\n", mysql_error(_QueryDatabaseInstanceMysql));
		return false;
	}

	return true;
}

DLLAPI bool RunDBQueryWithResult(const char *stmt, CDBResult *out)
{
	MYSQL_RES *res = NULL;

	if (!RunDBQuery(stmt))
		return false;

	res = mysql_use_result(_QueryDatabaseInstanceMysql);
	if (!res)
		return false;

	if (!out->Execute(res))
	{
		mysql_free_result(res);
		return false;
	}

	mysql_free_result(res);
	return true;
}


// CDBResult
DLLAPI CDBResult::CDBResult()
{
	m_totalrows = m_processrow = 0;
	m_fields = 0;
}

DLLAPI CDBResult::~CDBResult()
{
	m_rows.clear();
}

bool DLLAPI CDBResult::Execute(MYSQL_RES *res)
{
	unsigned int i = 0;
	bool bExec = true;
	MYSQL_ROW row;

	if (!res)
		return false;

	m_fields = mysql_num_fields(res);

	do
	{
		row = mysql_fetch_row(res);
		if (row)
		{
			std::vector<std::string> vec;
	
			for (i = 0; i < m_fields; i++)
				vec.push_back(row[i]);

			m_rows.push_back(vec);
			m_processrow++;
		}
		else
		{
			bExec = false;
		}
	} while (bExec);

	m_totalrows = mysql_num_rows(res);
	
	return true;
}

my_ulonglong DLLAPI CDBResult::GetTotalRows() { return m_totalrows; }
my_ulonglong DLLAPI CDBResult::GetProcessedRows() { return m_processrow; }
unsigned int DLLAPI CDBResult::GetFieldsCount() { return m_fields; }

std::vector<std::string> DLLAPI CDBResult::GetRow(my_ulonglong index)
{
	if (index > m_rows.size())
		return _Dummy;

	return m_rows.at((size_t)index);
}

DLLAPI std::string CDBResult::GetColumnByRow(my_ulonglong row, unsigned int column)
{
	std::vector<std::string> vec = GetRow(row);

	if (vec.size() < 1)
		return NULL;

	return vec.at((size_t)column);
}

DLLAPI unsigned long EscapeSQLString(const char *from, char *to, unsigned long length)
{
	return mysql_real_escape_string(_QueryDatabaseInstanceMysql, to, from, length);
}
