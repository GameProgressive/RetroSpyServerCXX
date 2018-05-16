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

DLLAPI std::string EscapeSQLString(MYSQL* con, std::string str)
{
	char *x = (char*)malloc(sizeof(char)*(str.length()*2+5));
	if (!x)
		return "";
	
	mysql_real_escape_string(con, x, str.c_str(), str.length());
	std::string k = std::string(x);
	
	free(x);
	return k;
}

DLLAPI void EscapeSQLString(MYSQL* c, std::string &str)
{
	str = EscapeSQLString(c, str.c_str());
}

DLLAPI bool RunDBQuery(MYSQL* con, std::string str)
{
	if (mysql_query(con, str.c_str()) != 0)
	{
		printf("[Query] Cannot execute query. Error: %s\n", mysql_error(con));
		return false;		
	}
	
	return true;
}

DLLAPI bool RunDBQuery(MYSQL *con, std::string query, ResultSet **rs)
{
	*rs = new ResultSet();
	if (!(*rs)->executeQuery(con, query))
	{
		delete rs;

		printf("[Query] Cannot execute query. Error: %s\n",  mysql_error(con));
		return false;		
	}

	return true;
}

DLLAPI ResultSet::ResultSet()
{
	m_pos = 0;
}

DLLAPI ResultSet::~ResultSet()
{
	size_t i = 0;
	
	for (; i < m_rows.size(); i++)
	{
		m_rows.at(i).clear();
	}
	
	m_rows.clear();
}

DLLAPI bool ResultSet::executeQuery(MYSQL* con, std::string str)
{
	MYSQL_RES *result = NULL;
	MYSQL_ROW row;
	unsigned int fields = 0;
	
	if (mysql_query(con, str.c_str()) != 0)
	{
		printf("[Query] Cannot execute query. Error: %s\n", mysql_error(con));
		return false;		
	}
	
	result = mysql_store_result(con);
	if (!result)
	{
		printf("[Query] Cannot execute query. Error: %s\n", mysql_error(con));
		return false;				
	}
	
	if (result->row_count == 0)
	{
		mysql_free_result(result);
		return true;
	}
	
	fields = mysql_num_fields(result);
	if (fields == 0)
	{
		mysql_free_result(result);
		return true;		
	}
	
	row = mysql_fetch_row(result);
	if (row == NULL)
	{
		mysql_free_result(result);
		printf("[Query] Cannot execute query. Error: %s\n", mysql_error(con));
		return false;			
	}
	
	do
	{
		std::vector<std::string> vec;
		unsigned int i =  0;
		
		for (; i < fields; i++)
			vec.push_back(row[i]);
		
		m_rows.push_back(vec);
	} while (row);
	
	mysql_free_result(result);
	
	return true;
}

size_t ResultSet::getRows()
{
	return m_rows.size();
}

bool ResultSet::first()
{
	if (m_rows.size() < 1)
		return false;
	
	m_pos = 0;
	return true;
}

bool ResultSet::next()
{
	m_pos++;
	
	if (m_rows.size() < (m_pos+1))
	{
		m_pos = 0;
		return false;
	}
	
	return true;
}

double ResultSet::getDouble(size_t index)
{
	if (m_rows.at(m_pos).size() < index)
		return 0;
	
	return std::stof(m_rows.at(m_pos).at(index));
}

unsigned int atoui(const char *x)
{
	return (unsigned int) strtol (x, (char **) NULL, 10);
}

DLLAPI unsigned int ResultSet::getUInt(size_t index)
{
	if (m_rows.at(m_pos).size() < index)
		return 0;
	
	return atoui(m_rows.at(m_pos).at(index).c_str());
}

DLLAPI std::string ResultSet::getString(size_t index)
{
	if (m_rows.at(m_pos).size() < index)
		return 0;
	
	return m_rows.at(m_pos).at(index);
}

DLLAPI int ResultSet::getInt(size_t index)
{
	if (m_rows.at(m_pos).size() < index)
		return 0;
	
	return std::stoi(m_rows.at(m_pos).at(index));
}