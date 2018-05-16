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
#include "RConfig.h"

#include "../common/Defines.h"

#include "INIReader.h"

#include <stdio.h>

const char *CConfig::GetDatabaseName()
{
	return m_szDBName;
}

int CConfig::GetDatabasePort()
{
	return m_DBPort;
}

const char *CConfig::GetDatabaseUsername()
{
	return m_szDBUser;
}

const char *CConfig::GetDatabasePassword()
{
	return m_szDBPass;
}

const char *CConfig::GetDatabaseHost()
{
	return m_szDBHost;
}

const char *CConfig::GetDefaultIP()
{
	return m_szDIP;
}

const char *CConfig::GetDatabaseSocket()
{
	return m_szDBSock;
}

bool CConfig::Load(CModuleManager *mngr, const char *name)
{
	// Create an INI instance
	INIReader reader(name);

	size_t i = 0, k = 0;

	bool bC = true;

	std::string str = "";

	// Load the INI
	if (reader.ParseError() < 0)
		return false;

	// Load the Database section
	m_DBPort = reader.GetInteger("Database", "Port", 3306);

	strcpy_s(m_szDBName, MAX_INI_BUFFER, reader.Get("Database", "Name", "gamespy").c_str());
	strcpy_s(m_szDBPass, MAX_INI_BUFFER, reader.Get("Database", "Password", "").c_str());
	strcpy_s(m_szDBUser, MAX_INI_BUFFER, reader.Get("Database", "Username", "gamespy").c_str());
	strcpy_s(m_szDBHost, MAX_INI_BUFFER, reader.Get("Database", "Host", "localhost").c_str());
	strcpy_s(m_szDBSock, MAX_INI_BUFFER, reader.Get("Database", "Socket", "").c_str());

	strcpy_s(m_szDIP, MAX_INI_BUFFER, reader.Get("Server", "DefaultIP", "localhost").c_str());

	// Load the modules
	while (bC)
	{
		char name[21];

		// Set the name
		sprintf_s(name, 20, "Load%u", i);

		// Try to get the module name
		str = reader.Get("Modules", name, "NOT_FOUND");
		
		if (str ==  "NOT_FOUND")
			bC = false; // This is the last module, exit
		else
		{
			// Get the fields of the module section
			std::set<std::string> set = reader.GetFields(str);

			if (set.size() < 1)
				// Just load without the configuration if we don't have any
				mngr->LoadRetroSpyModule(str.c_str());
			else
			{
				ModuleConfigMap map;

				for (k = 0; k < set.size(); k++)
				{
					// Get the string on the specific position
					std::set<std::string>::iterator it = set.begin();
					std::advance(it, k);

					// Set the map value to the field
					map[*it] = reader.Get(str, *it, "");
				}

				// Load the module with the config
				mngr->LoadRetroSpyModule(str.c_str(), map);
			}
		}

		// Advance to the next load
		i++;
	}

	return true;
}

int CConfig::m_DBPort = 0;
char CConfig::m_szDBHost[MAX_INI_BUFFER+1] = {0};
char CConfig::m_szDBUser[MAX_INI_BUFFER+1] = {0};
char CConfig::m_szDBPass[MAX_INI_BUFFER+1] = {0};
char CConfig::m_szDBSock[MAX_INI_BUFFER+1] = {0};
char CConfig::m_szDBName[MAX_INI_BUFFER+1] = {0};
char CConfig::m_szDIP[MAX_INI_BUFFER+1] = {0};
