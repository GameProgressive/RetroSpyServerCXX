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

#include <TemplateStringServer.h>
#include <MDK/Database.h>

/*
	This class rappresents a Player Search server
*/
class PSServer : public CTemplateStringServer
{
public:
	PSServer(int defaultport, bool udp);
	~PSServer();
	
	int Initialize();

	/* See CServer::OnNewConnection */
	bool OnTCPNewConnection(mdk_socket stream, int status);

	/* See CStringServer::HandleRequest */
	bool HandleRequest(mdk_socket stream, const char *req, const char *buf, int size);

private:
	bool OnValid(mdk_socket stream, const char *buf, int size);
	bool OnSendNicks(mdk_socket stream, const char *buf, int size);
	bool OnCheckNicks(mdk_socket stream, const char *buf, int size);
	bool OnSearchUsers(mdk_socket stream, const char *buf, int size);
	bool OnReverseBuddies(mdk_socket stream, const char *buf, int size);
	bool OnOthersList(mdk_socket stream, const char *buf, int size);
	bool OnUniqueSearch(mdk_socket stream, const char *buf, int size);
	bool OnProfileList(mdk_socket stream, const char *buf, int size);
	bool OnProductMatching(mdk_socket stream, const char *buf, int size);
	bool OnNewUser(mdk_socket stream, const char *buf, int size);
};

#endif
