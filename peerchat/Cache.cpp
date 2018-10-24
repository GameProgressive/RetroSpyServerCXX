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
#include "Cache.h"

#include <MDK/Query.h>

#include <limits.h>

CCache::CCache()
{
	m_instance = this;
	m_sReservedClientID = 0;
}

CCache::~CCache()
{
	TClientMap::iterator it = m_clients.begin();
	for (; it != m_clients.end(); it++)
	{
		delete (*it).second;
	}
	
	m_clients.clear();
}

bool CCache::AddUser(mdk_socket socket)
{
	if (m_sReservedClientID == SIZE_MAX)
		return false; // Reached max clients supported
	
	CIRCClient* cc = new CIRCClient(socket, m_sReservedClientID);
	if (!cc)
		return false;
	
	m_sReservedClientID++;
	return true;
}

CIRCClient* CCache::GetUser(unsigned long long id)
{
	
}

void CCache::DelUser(unsigned long long id)
{
	
}

bool CCache::LoadData()
{
	CResultSet* res = new CResultSet();
}

CCache* CCache::m_instance = NULL;
