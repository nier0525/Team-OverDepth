#include "ListenSocket.h"

CListenSocket::CListenSocket()
{
	sock = NULL;
}

CListenSocket::~CListenSocket()
{
	Release();
}

void CListenSocket::TCP_Setting(const char* _id, const short _port)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) CError::GetInstance()->err_quit("socket");

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;

	if (_id == nullptr)
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr(_id);
	}

	addr.sin_port = htons(_port);

	int retval = bind(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR) CError::GetInstance()->err_quit("bind");
}

void CListenSocket::Listen()
{
	int retval = listen(sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) CError::GetInstance()->err_quit("listen");
}

void CListenSocket::Listen(const int _count)
{
	int retval = listen(sock, _count);
	if (retval == SOCKET_ERROR) CError::GetInstance()->err_quit("listen");
}

SOCKET CListenSocket::Accept()
{
	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);

	SOCKET client_sock = accept(sock, (SOCKADDR*)&addr, &addrlen);
	if (client_sock == INVALID_SOCKET) CError::GetInstance()->err_display("Accept");

	return client_sock;
}

void CListenSocket::Release()
{
	closesocket(sock);
}