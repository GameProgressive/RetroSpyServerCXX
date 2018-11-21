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
#include <Helper.h>
#include <limits.h>
#include <string.h>


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
	
	TChannelVector::iterator it2 = m_channels.begin();
	for (; it2 != m_channels.end(); it2++)
	{
		delete (*it2);
	}
	
	m_channels.clear();
	m_clients.clear();
	
	m_vUserProps.clear();
	m_vGameClients.clear();
	m_vChannelProps.clear();
	
	m_instance = NULL;
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

bool CCache::LoadData(CDatabase* db)
{
	CResultSet* res = new CResultSet();
	
	if (res->ExecuteQuery(db, "SELECT `key`,`mask`,`comment`,`entrymsg`,UNIX_TIMESTAMP(`expires`), `groupname`, `limit`, `mode`, `onlyowner`, `setbynick`, `setbypid`, UNIX_TIMESTAMP(`setondate`), `topic`, `setbyhost` FROM `channel_properties"))
	{
		if (res->GotoFirstRow())
		{
			do
			{
				SChannelProps props;
				memset(&props, 0, sizeof(props));
				strncpy(props.key, res->GetStringFromRow(0).c_str(), MAX_NAME+1);
				strncpy(props.mask, res->GetStringFromRow(1).c_str(), MAX_NAME+1);
				strncpy(props.comment, res->GetStringFromRow(2).c_str(), MAX_COMMENT + 1);
				strncpy(props.entrymsg, res->GetStringFromRow(3).c_str(), MAX_COMMENT + 1);
				props.expires = res->GetIntFromRow(4);
				strncpy(props.groupname, res->GetStringFromRow(5).c_str(), MAX_NAME + 1);
				props.limit = res->GetIntFromRow(6);
				strncpy(props.modes, res->GetStringFromRow(7).c_str(), MAX_NAME + 1);
				props.onlyowner = res->GetIntFromRow(8);
				
				if (res->GetStringFromRow(9).c_str()[0] != 0)
					strncpy(props.setbynick, res->GetStringFromRow(9).c_str(), MAX_NAME + 1);
				else
					strncpy(props.setbynick, "SERVER", MAX_NAME + 1);
				
				props.setbypid = res->GetUIntFromRow(10);
				props.setondate = res->GetIntFromRow(11);
				strncpy(props.topic, res->GetStringFromRow(12).c_str(), MAX_COMMENT + 1);
				strncpy(props.setbyhost, res->GetStringFromRow(13).c_str(), MAX_NAME + 1);

				m_vChannelProps.push_back(props);
				UpdateChannelProperties(props, false);				
			} while (res->GotoNextRow());
		}
	}
	
	if (res->ExecuteQuery(db, "SELECT `mask`,`comment`,UNIX_TIMESTAMP(`expires`),`hostmask`,`machineid`,`modeflags`,`profileid`,`setbyhost`,`setbynick`,`setbypid`,UNIX_TIMESTAMP(`setondate`),`usermodeid` FROM `channel_user_properties`"))
	{
		if (res->GotoFirstRow())
		{
			do
			{
				SUserProps usermode;
				memset(&usermode,0,sizeof(usermode));
				strncpy(usermode.mask, res->GetStringFromRow(0).c_str(), MAX_NAME+1);
				strncpy(usermode.comment, res->GetStringFromRow(1).c_str(), MAX_COMMENT+1);
				usermode.expires = res->GetIntFromRow(2);
				strncpy(usermode.hostmask, res->GetStringFromRow(3).c_str(), MAX_NAME+1);
				strncpy(usermode.machineid, res->GetStringFromRow(4).c_str(), MAX_NAME+1);
				usermode.modeflags = res->GetIntFromRow(5);
				usermode.profileid = res->GetIntFromRow(6);
				strncpy(usermode.setbyhost, res->GetStringFromRow(7).c_str(), MAX_NAME+1);
				strncpy(usermode.setbynick, res->GetStringFromRow(8).c_str(), MAX_NAME+1);
				usermode.setbypid = res->GetIntFromRow(9);
				usermode.setondate = res->GetIntFromRow(10);
				usermode.usermodeid  = res->GetIntFromRow(11);
				usermode.isGlobal = 1;
				
				m_vUserProps.push_back(usermode);
				UpdateUserModes(usermode);
			} while (res->GotoNextRow());
	   }
	}
	
	if (res->ExecuteQuery(db, "SELECT `mask`,`gameid` FROM `channel_clients`"))
	{
		if (res->GotoFirstRow())
		{
			do
			{
				SGameClient client;
				memset(&client,0,sizeof(client));
				
				strncpy(client.mask, res->GetStringFromRow(0).c_str(), res->GetStringFromRow(0).length() % sizeof(client.mask));
				client.gameid = res->GetIntFromRow(1);
				
				m_vGameClients.push_back(client);				
			} while (res->GotoNextRow());
		}
	}
	
	if (res->ExecuteQuery(db, "SELECT `profileid`, `rightsmask` FROM `operators`"))
	{
		if (res->GotoFirstRow())
		{
			do
			{
				AddOperator(res->GetUIntFromRow(0), res->GetUIntFromRow(1));
			} while (res->GotoNextRow());
		}
	}
	
	delete res;
	return true;
}

void CCache::UpdateChannelProperties(SChannelProps props, bool kickexisting)
{
	TChannelVector::iterator it = m_channels.begin();
	CIRCChannel* chan = NULL;
	
	if (strchr(props.modes, 'z') != NULL && strchr(props.mask, '*') == NULL)
	{
		if (FindChannel(props.mask) == NULL)
		{
			chan = new CIRCChannel(props.mask);
			chan->UpdateChannelProperties(props, false);
			return;
		}
	}
	
	while (it != m_channels.end())
	{
		chan = *it;
		
		if (match_mask_with_name(props.mask, (const char*)chan->GetName()) == 0)
		{
			if (GetClosestChannelProperties(chan->GetName()) == props)
				chan->UpdateChannelProperties(props, kickexisting);
		}
		
		it++;
	}
}

CCache* CCache::m_instance = NULL;
