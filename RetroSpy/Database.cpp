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
#include "Config.h"

#include <stdio.h>

CDatabase::CDatabase()
{
	mysql_init(&m_db);
}

CDatabase::~CDatabase()
{
	Disconnect();
}

bool CDatabase::Connect()
{
	char *pass = (char*)CConfig::GetDatabasePassword();
	char *sock = (char*)CConfig::GetDatabaseSocket();

	if (strlen(pass) < 1)
		pass = NULL;

	if (strlen(sock) < 1)
		sock = NULL;

	if (!mysql_real_connect(&m_db,
		CConfig::GetDatabaseHost(),
		CConfig::GetDatabaseUsername(),
		pass,
		CConfig::GetDatabaseName(),
		CConfig::GetDatabasePort(),
		sock,
	0))
	{
		printf("[Database] Cannot connect. Error: %s\n", mysql_error(&m_db));
		return false;
	}

	SetDBInstance(&m_db);

	return true;
}

void CDatabase::Disconnect()
{
	mysql_close(&m_db);
}
