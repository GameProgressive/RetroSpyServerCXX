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
#include "Defines.h"
#include <stdio.h>

#include "PSServer.h"

#define DEFAULT_PORT 29901

/* Entry point for GPSP server */
RetroSpyModuleStart
	ModuleMain *mm = (ModuleMain*)data;
	PSServer *Server = NULL;

	if (mm->port == -1)
		mm->port = DEFAULT_PORT;
	
	if (mm->mysql == NULL)
	{
		LOG_ERROR("PlayerSearch", "Unable to retrive MySQL connection!");
		return ERROR_MYSQL_POINTER;
	}

	Server = new PSServer(mm->mysql);

	if (!Server->Bind(mm->ip, mm->port, false))
		return ERROR_BIND_ERROR;

	LOG_INFO("PlayerSearch", "Server started on %s:%d!", mm->ip, mm->port);

	return ERROR_NONE;
RetroSpyModuleEnd
