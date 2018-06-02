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
#ifndef GPSPSERVER_H
#define GPSPSERVER_H

#include <StringServer.h>

/*
	This class rappresents a Player Search server
*/
class PSServer : public CStringServer
{
public:
	PSServer(mdk_mysql* con);
	~PSServer();

	/* See CServer::OnNewConnection */
	bool OnNewConnection(mdk_client* stream);

	/* See CStringServer::HandleRequest */
	bool HandleRequest(mdk_client *stream, const char *req, const char *buf, int size);

private:
	bool OnValid(mdk_client *stream, const char *buf, int size);
	bool OnSendNicks(mdk_client *stream, const char *buf, int size);
	bool OnCheckNicks(mdk_client *stream, const char *buf, int size);
	bool OnSearchUsers(mdk_client *stream, const char *buf, int size);
	bool OnReverseBuddies(mdk_client *stream, const char *buf, int size);
	bool OnOthersList(mdk_client *stream, const char *buf, int size);
	bool OnUniqueSearch(mdk_client *stream, const char *buf, int size);
	bool OnProfileList(mdk_client *stream, const char *buf, int size);
	bool OnProductMatching(mdk_client *stream, const char *buf, int size);
	
	mdk_mysql *m_con;

};

#endif
