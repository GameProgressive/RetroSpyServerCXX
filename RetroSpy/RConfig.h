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
#ifndef RETROSPY_CONFIG_H
#define RETROSPY_CONFIG_H

#include "ModuleManager.h"

class CConfig
{
public:
	static const char *GetDatabaseSocket();
	static const char *GetDefaultIP();
	static const char *GetDatabaseHost();
	static const char *GetDatabasePassword();
	static int GetDatabasePort();
	static const char *GetDatabaseName();
	static const char *GetDatabaseUsername();
	
	static bool Load(CModuleManager *mngr, const char *name);
private:
	static int m_DBPort;
	static char m_szDBHost[MAX_INI_BUFFER+1];
	static char m_szDBUser[MAX_INI_BUFFER+1];
	static char m_szDBPass[MAX_INI_BUFFER+1];
	static char m_szDBSock[MAX_INI_BUFFER+1];
	static char m_szDBName[MAX_INI_BUFFER+1];
	static char m_szDIP[MAX_INI_BUFFER+1];

};

#endif
