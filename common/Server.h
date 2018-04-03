/*
    This file is part of RetroSpy Server.

    RetroSpy Server is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef RETROSPYCOMMON_SERVER_H
#define RETROSPYCOMMON_SERVER_H

#include "Loop.h"

#include <string>

class CServer;

typedef struct SServerData
{
	uv_loop_t *loop;
	CServer *instance;
} TServerData;

/*
	This class rapresents a libuv implemented TCP or UDP Server
*/
class CServer
{
public:
	DLLAPI CServer(CLoop* loop);
	DLLAPI ~CServer();

	/*
		Function: Bind
		Description: Bind the socket
		Return: true if the server binded, otherwise false
		Parameters:
			ip => The ip to bind
			port => The port to bind
			udp => True if this is a UDP server
	*/
	bool DLLAPI Bind(const char *ip, int port, bool udp);

	/*
		Function: Write
		Description: Write something to a socket
		Parameters:
			client => the client
			data => the data to write
			size => size of the data to write
	*/
	static void DLLAPI Write(uv_stream_t *client, void *data, int size);

	/*
		Function: Write
		Description: Write a set of characters to a socket
		Parameters:
			client => the client
			data => the data to write
	*/
	static void DLLAPI Write(uv_stream_t *client, const char *data);

	/*
		Function: Write
		Description: Write a string to a socket
		Parameters:
			client => the client
			data => the string that will be written
	*/
	static void DLLAPI Write(uv_stream_t *client, std::string data);

	/*
		Function: OnRead
		Description: This function is called when the server is processing
			readed data from the client
		Parameters:
			stream => the client
			data => the data readed
			size => the size of the data
		NOTE: This function must be overwritten by an implementation of the Server
	*/
	virtual void DLLAPI OnRead(uv_stream_t* stream, const char *data, ssize_t size) = 0;

	/*
		Function: OnNewConnection
		Description: This function is called when a new connection is processed
		Parameters:
			stream => the client that is connected
		NOTE: This function must be overwritten by an implementation of the Server
	*/
	virtual bool DLLAPI OnNewConnection(uv_stream_t* stream) = 0;

	/*
		Function: Close
		Description: Close a client connection
		Parameters:
			client => the client
	*/
	void DLLAPI Close(uv_stream_t *client);

	/*
		Function: Close
		Description: Close a handle
		Parameters:
			handle => the handle to close
	*/
	void DLLAPI Close(uv_handle_t *handle);

private:
	uv_loop_t *m_loop;

	uv_tcp_t m_tcp;
	uv_udp_t m_udp;

	TServerData m_data;
};

/*
	This class rapresents a generic client data
	that are passed throught all the clients
*/
class CClientData
{
public:
	DLLAPI CClientData(CServer *instance);
	DLLAPI ~CClientData();

	/*
		Function: GetInstance
		Description: Get the server instance
		Return: A pointer to the server instance
	*/
	DLLAPI CServer *GetInstance();
	
	/*
		Function: GetUserData
		Description: Get the user data
		Return: A pointer to the user data
	*/
	DLLAPI void *GetUserData();

	/*
		Function: SetUserData
		Description: Set user data that could be passed throught the socket
		Parameters:
			data => A pointer to the data that will be passed
	*/
	DLLAPI void SetUserData(void *data);

protected:
	CServer *m_instance;
	void *m_data;
};

#endif
