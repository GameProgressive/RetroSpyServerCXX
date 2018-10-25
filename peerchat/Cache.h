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
#ifndef _CACHE_H_
#define _CACHE_H_

#include "IRCClient.h"
#include "IRCChannel.h"
#include "Structs.h"

#include <MDK/Database.h>

#include <vector>
#include <map>

typedef std::map<unsigned long long, CIRCClient*> TClientMap;
typedef std::vector<CIRCChannel*> TChannelVector;

class CCache
{
public:
		CCache();
		~CCache();
		
		bool LoadData(CDatabase * db);
		
		bool AddUser(mdk_socket socket);
		CIRCClient* GetUser(unsigned long long id);
		void DelUser(unsigned long long id);
		
		inline static CCache* Instance() { return m_instance; }
		
		CIRCChannel* FindChannel(const char* name);
		
		void UpdateChannelProperties(SChannelProps props, bool kickexisting);	
		void UpdateUserModes(SUserProps props);
		
protected:
		TClientMap m_clients;
		TChannelVector m_channels;
		
		std::vector<SUserProps> m_vUserProps;
		std::vector<SChannelProps> m_vChannelProps;
		std::vector<SGameClient> m_vGameClients;
		
		unsigned long long m_sReservedClientID;

		void AddOperator(unsigned int id, unsigned int mask);
		SChannelProps GetClosestChannelProperties(const char* name);
		
private:
		static CCache* m_instance;
};

#endif
