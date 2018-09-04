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
#include "QR2Server.h"

#include <MDK/Utility.h>

QR2Server::QR2Server(CDatabase* db)
{
	m_dbConnect = db;
}

QR2Server::~QR2Server()
{
}

void QR2Server::OnUDPRead(mdk_socket client, const struct sockaddr* addr, const char *data, ssize_t size)
{
	LOG_INFO("QR2Server", "Recv UDP!!!");
}

