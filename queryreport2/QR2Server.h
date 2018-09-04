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
#ifndef QR2SERVER_H
#define QR2SERVER_H

#include <Defines.h>
#include <MDK/TemplateServer.h>
#include <MDK/Query.h>

/*
This class rappresents a Query Report 2 Server
*/
class QR2Server : public CTemplateServer
{
public:
	QR2Server(CDatabase* db);
	~QR2Server();

	/* See CServer::OnNewConnection */
	bool OnTCPNewConnection(mdk_socket stream, int status) { return true; }

	void OnTCPRead(mdk_socket client, const char *data, ssize_t size) {}
	void OnUDPRead(mdk_socket client, const struct sockaddr* addr, const char *data, ssize_t size);
private:
	CDatabase* m_dbConnect;
};

#endif
