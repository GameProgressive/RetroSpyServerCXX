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
#define RSC_EXPORT 1 //Export the methods
#include "Server.h"

// Defines
#define DEFAULT_BACKLOG 128

#ifdef _WIN32
	#define s_addr S_un.S_addr

int uv_udp_getpeername(const uv_udp_t* handle,
                       struct sockaddr* name,
                       int* namelen) {
  int result;

  if (handle->socket == INVALID_SOCKET) {
    return UV_EINVAL;
  }

  result = getpeername(handle->socket, name, namelen);
  if (result != 0) {
    return uv_translate_sys_error(WSAGetLastError());
  }

  return 0;
}
#else
int uv_udp_getpeername(const uv_udp_t* handle,
                       struct sockaddr* name,
                       int* namelen) {
  socklen_t socklen;

  if (uv__stream_fd(handle) < 0)
    return UV_EINVAL;  /* FIXME(bnoordhuis) UV_EBADF */

  /* sizeof(socklen_t) != sizeof(int) on some systems. */
  socklen = (socklen_t) *namelen;

  if (getpeername(uv__stream_fd(handle), name, &socklen))
    return UV__ERR(errno);

  *namelen = (int) socklen;
  return 0;
}
#endif

// Prototypes
void _OnTCPNewConnection(uv_stream_t *server, int status);
void _OnUDPNewConnection(uv_stream_t *server, int status);

void _AllocBuffer(uv_handle_t *handle, size_t size, uv_buf_t* buf);
void _OnRead(uv_stream_t *stream, ssize_t size, const uv_buf_t *buf);
void _OnClose(uv_handle_t *handle);
void _OnWrite(uv_write_t* req, int status);

DLLAPI CServer::CServer(CLoop *loop)
{
	m_loop = loop->GetLoopPtr();
}

DLLAPI CServer::~CServer()
{

}

bool DLLAPI CServer::Bind(const char *ip, int port, bool udp)
{
	int r = 0;
	struct sockaddr_in addr;

	m_data.loop = m_loop;
	m_data.instance = this;

	// Initialize the socket

	if (udp)
		uv_udp_init(m_loop, &m_udp);
	else
		uv_tcp_init(m_loop, &m_tcp);

	// Resolve ip and port
	uv_ip4_addr(ip, port, &addr);

	if (udp)
		uv_udp_bind(&m_udp, (const struct sockaddr*)&addr, 0);
	else
		uv_tcp_bind(&m_tcp, (const struct sockaddr*)&addr, 0);

	/* Listen the socket */
	if (udp)
		r = uv_listen((uv_stream_t*)&m_udp, DEFAULT_BACKLOG, _OnUDPNewConnection);
	else
		r = uv_listen((uv_stream_t*)&m_tcp, DEFAULT_BACKLOG, _OnTCPNewConnection);

	// Access the loop from the other functions
	if (udp)
		m_udp.data = (void*)&m_data;
	else
		m_tcp.data = (void*)&m_data;

	if (r)
	{
		printf("[Server] %s listen error: %s\n", udp ? "UDP" : "TCP", uv_strerror(r));
		return false;
	}

	return true;
}

void DLLAPI CServer::Write(uv_stream_t *client, void *data, int size)
{
	uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
	uv_buf_t buf;

	buf.len = size;
	buf.base = (char*)data;

	uv_write(req, client, &buf, 1, _OnWrite);
}

void DLLAPI CServer::Write(uv_stream_t *client, std::string str)
{
	Write(client, (void*)str.c_str(), str.length());
}

void DLLAPI CServer::Write(uv_stream_t *client, const char* data)
{
	Write(client, (void*)data, strlen(data));
}

void DLLAPI CServer::Close(uv_stream_t *client)
{
	Close((uv_handle_t*)client);
}

void DLLAPI CServer::Close(uv_handle_t* handle)
{
	uv_close(handle, _OnClose);
}

int DLLAPI CServer::GetIPFromStream(uv_stream_t *stream)
{
	struct sockaddr_in clientaddr;
	socklen_t clientaddr_len;
	CClientData *data = NULL;

	if (!stream->data)
		return -1;

	data = (CClientData*)stream->data;

	if (data->IsUDP())
		uv_udp_getpeername((const uv_udp_t*)stream, (struct sockaddr*)&clientaddr, (int*)&clientaddr_len);
	else
		uv_tcp_getpeername((const uv_tcp_t*)stream, (struct sockaddr*)&clientaddr, (int*)&clientaddr_len);

	return clientaddr.sin_addr.s_addr;
}

// Pure virtual functions
void DLLAPI CServer::OnRead(uv_stream_t*, const char *, ssize_t) {}
bool DLLAPI CServer::OnNewConnection(uv_stream_t*) { return true; }

// UV Callbacks
void _OnClose(uv_handle_t* handle)
{
	if (handle)
	{
		// Free CClientData
		if (handle->data)
			delete handle->data;

		free(handle);
	}
}

void _OnWrite(uv_write_t* req, int)
{
	if (req)
		free(req);
}

void _AllocBuffer(uv_handle_t*, size_t size, uv_buf_t *buf)
{
	*buf = uv_buf_init((char*)malloc(size), size);
}

void _OnRead(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
	CClientData *data = NULL;

	if (nread < 0 || stream->data == NULL)
	{
		uv_close((uv_handle_t*)stream, _OnClose);

		if (buf->base)
			free(buf->base);

		return;
	}
	else if (nread == 0)
	{
		if (buf->base)
			free(buf->base);

		return;
	}

	data = (CClientData*)stream->data;
	data->GetInstance()->OnRead(stream, buf->base, nread);
	
	if (buf->base)
		free(buf->base);
}

// TCP Callbacks
void _OnTCPNewConnection(uv_stream_t *server, int status)
{
	uv_tcp_t *client = NULL;
	TServerData *data = NULL;

	if (status < 0 || server->data == NULL)
		return;

	data = (TServerData*)server->data;

	client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));

	uv_tcp_init(data->loop, client);

	// Accept the client
	status = uv_accept(server, (uv_stream_t *)client); 

	if (status != 0)
	{
		uv_close((uv_handle_t*)client, _OnClose);
	}

	client->data = (void*)new CClientData(data->instance, false);
	
	if (!data->instance->OnNewConnection((uv_stream_t*)client))
	{
		uv_close((uv_handle_t*)client, _OnClose);
		return;
	}

	status = uv_read_start((uv_stream_t*)client, _AllocBuffer, _OnRead);

	if (status != 0)
		uv_close((uv_handle_t*)client, _OnClose);
}

// UDP Callbacks
void _OnUDPNewConnection(uv_stream_t *server, int status)
{
	uv_udp_t *client = NULL;
	TServerData *data = NULL;

	if (status < 0 || server->data == NULL)
		return;

	data = (TServerData*)server->data;

	client = (uv_udp_t *)malloc(sizeof(uv_udp_t));

	// Accept the client
	uv_udp_init(data->loop, client);

	if (status != 0)
	{
		uv_close((uv_handle_t*)client, _OnClose);
	}

	client->data = (void*)new CClientData(data->instance, true);
	
	if (!data->instance->OnNewConnection((uv_stream_t*)client))
	{
		uv_close((uv_handle_t*)client, _OnClose);
		return;
	}

	status = uv_read_start((uv_stream_t*)client, _AllocBuffer, _OnRead);

	if (status != 0)
		uv_close((uv_handle_t*)client, _OnClose);
}

// CClientData
DLLAPI CClientData::~CClientData() {}

DLLAPI CClientData::CClientData(CServer *instance, bool udp)
{
	m_instance = instance;
	m_data = NULL;
	m_udp = udp;
}

DLLAPI CServer* CClientData::GetInstance()
{
	return m_instance;
}

DLLAPI void *CClientData::GetUserData()
{
	return m_data;
}

DLLAPI void CClientData::SetUserData(void *data)
{
	m_data = data;
}

DLLAPI bool CClientData::IsUDP()
{
	return m_udp;
}
