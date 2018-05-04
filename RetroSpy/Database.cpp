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

#include <cppconn\exception.h>

CDatabase::CDatabase()
{
	m_driver = get_driver_instance();
	m_connection = NULL;
}

CDatabase::~CDatabase()
{
	Disconnect();
}

bool CDatabase::Connect()
{
	std::string ppt = "";

	char *pass = (char*)CConfig::GetDatabasePassword();
	char *sock = (char*)CConfig::GetDatabaseSocket();

	if (strlen(pass) < 1)
		pass = NULL;

	if (strlen(sock) < 1)
		sock = NULL;

	if (sock)
	{
		ppt = "unix://";
		ppt += sock;
	}

	if (!sock)
	{
		ppt = "tcp://";
		ppt += CConfig::GetDatabaseHost();
		ppt += ":";
		ppt += std::to_string(CConfig::GetDatabasePort());
	}

	ppt += "/";
	ppt += CConfig::GetDatabaseName();

	try
	{
		m_connection = m_driver->connect(ppt, CConfig::GetDatabaseUsername(), pass);

		if (!m_connection)
			return false;
	}
	catch (sql::SQLException &ex)
	{
		printf("[Database] Cannot connect. Error: %s\n", ex.what());
		return false;
	}

	SetConnectionPtr(m_connection);

	return true;
}

void CDatabase::Disconnect()
{
	if (m_connection)
		delete m_connection;

	m_connection = NULL;
}
