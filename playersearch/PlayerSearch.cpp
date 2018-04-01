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
	int port = 0;

	// Get config
	{
		ModuleConfigMap::iterator it = mm->cfg.find("Port");
		
		if (it == mm->cfg.end())
			port = DEFAULT_PORT;
		else
		{
			port = atoi(it->second.c_str());
		}
	}

	CLoop loop;
	PSServer *Server = new PSServer(&loop);

	if (!Server->Bind(mm->default_ip, port, false))
		return 1;

	printf("[PlayerSearch] Started on %s:%d\n", mm->default_ip, port);

	loop.Run();

	return 0;
}
