/*
    This file is part of RetroSpy Server.

    RetroSpy Server is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RetroSpy Server is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSEc  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with RetroSpy Server.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "NNServer.h"
#include "ClientManager.h"

#include <MDK/ModuleEntryPoint.h>
#include <MDK/Utility.h>

CNNServer::CNNServer(int defaultport, bool udp) : CThreadServer(defaultport, udp) {}

CNNServer::~CNNServer()
{
	CClientManager::Free();	
}

int CNNServer::Initialize()
{
	if (!m_lpDatabase)
		return ERROR_DATABASE_ERROR;

	ModuleConfigMap::iterator it = m_cfg.find("MatchIP");
	if (it == m_cfg.end())
	{
		LOG_ERROR("NatNeg", "Cannot find option \"MatchIP\"");
		return ERROR_CONFIGURATION_ERROR;
	}
	SetMatchIP(it->second.c_str());
	
	it = m_cfg.find("ProbeIP");
	if (it == m_cfg.end())
	{
		LOG_ERROR("NatNeg", "Cannot find option \"ProbeIP\"");
		return ERROR_CONFIGURATION_ERROR;
	}
	
	SetProbeIP(it->second.c_str());

	
	return ERROR_NONE;
}

void CNNServer::OnUDPRead(mdk_socket client, const struct sockaddr* addr, const char *data, ssize_t size)
{
	CClientManager::Handle(m_dbConnection, client, addr, data, size);
}

char CNNServer::m_matchIp[INET_ADDRSTRLEN] = {0};
char CNNServer::m_probeIp[INET_ADDRSTRLEN] = {0};

ModuleEntryPoint(CNNServer, MATCHUP_PORT, true)
