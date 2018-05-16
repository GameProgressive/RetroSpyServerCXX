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
#ifndef RETROSPYMODULEDEF_H
#define RETROSPYMODULEDEF_H

#include <string>
#include <map>
#include <mysql.h>

typedef std::map<std::string, std::string> ModuleConfigMap;

typedef struct SModuleMain
{
	char *ip;
	int port;
	ModuleConfigMap cfg;
	MYSQL* mysql;
} ModuleMain;


#endif
