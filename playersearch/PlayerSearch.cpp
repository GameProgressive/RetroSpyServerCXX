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
#include <stdio.h>

#include "../common/ModuleDef.h"

#include "PSServer.h"

#define DEFAULT_PORT 29901

/* Entry point for GPSP server */
int RetroSpyMain(void* data)
{
	ModuleMain *mm = (ModuleMain*)data;

	if (mm->port == -1)
		mm->port = DEFAULT_PORT;

	CLoop loop;
	PSServer *Server = new PSServer(&loop);

	if (!Server->Bind(mm->ip, mm->port, false))
		return 1;

	printf("[PlayerSearch] Started on %s:%d\n", mm->ip, mm->port);

	loop.Run();

	return 0;
}
