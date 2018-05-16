/*
    This file is part of RetroSpy Server.

    RetroSpy Server is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RetroSpy Server is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with RetroSpy Server.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Database.h"
#include "RConfig.h"

#include <stdio.h>

CDatabase::CDatabase()
{
	Init(&m_connection);
}

CDatabase::~CDatabase()
{
	Disconnect(&m_connection);
}

bool CDatabase::Connect()
{
	return Connect(&m_connection);
}

void CDatabase::Disconnect()
{
	Disconnect(&m_connection);
}

void CDatabase::Init(MYSQL *mysql)
{
	mysql_init(mysql);
}

bool CDatabase::Connect(MYSQL* mysql)
{
	char *pass = (char*)CConfig::GetDatabasePassword();
	char *sock = (char*)CConfig::GetDatabaseSocket();
	
	if (strlen(pass) < 1)
		pass = NULL;

	if (strlen(sock) < 1)
		sock = NULL;

	if (!mysql_real_connect(mysql, sock ? NULL : CConfig::GetDatabaseHost(), CConfig::GetDatabaseUsername(),
		pass, CConfig::GetDatabaseName(), CConfig::GetDatabasePort(), sock, 0))
	{
		printf("[Database] Cannot connect. Error: %s\n", mysql_error(mysql));
		return false;
	}

	return true;
}

void CDatabase::Disconnect(MYSQL *mysql)
{
	mysql_close(mysql);
}
