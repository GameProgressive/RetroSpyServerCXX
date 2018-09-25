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

#include "NNServer.h"

#include <MDK/Utility.h>
#include <MDK/ModuleEntryPoint.h>

#include <Defines.h>

#include "NNTypes.h"

/* Entry point for NatNeg server */
RetroSpyModuleStart
	ModuleMain *mm = (ModuleMain*)data;
	CNNServer *Server = NULL;
	ModuleConfigMap::iterator it;
	
	if (mm->port == -1)
		mm->port = MATCHUP_PORT;
	
	if (mm->db == NULL)
	{
		LOG_ERROR("NatNeg", "Unable to retrive Database connection!");
		return ERROR_MYSQL_POINTER;
	}
	
	Server = new CNNServer(mm->db);
	
	it = mm->cfg.find("MatchIP");
	if (it == mm->cfg.end())
	{
		LOG_ERROR("NatNeg", "Cannot find option \"MatchIP\"");
		return ERROR_BIND_ERROR;
	}
	Server->SetMatchIP(it->second.c_str());
	it = mm->cfg.find("ProbeIP");
	if (it == mm->cfg.end())
	{
		LOG_ERROR("NatNeg", "Cannot find option \"ProbeIP\"");
		return ERROR_BIND_ERROR;
	}
	Server->SetProbeIP(it->second.c_str());

	if (!Server->Bind(mm->ip, mm->port, true))
		return ERROR_BIND_ERROR;

	LOG_INFO("NatNeg", "Server started on %s:%d!", mm->ip, mm->port);

RetroSpyModuleEnd
