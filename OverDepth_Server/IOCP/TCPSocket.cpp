#include "TCPSocket.h"

CTCPSocket::CTCPSocket()
{
	sock = NULL;
	ZeroMemory(&addr, sizeof(addr));

	recv_overlapped.ptr = this;
	recv_overlapped.type = IO_TYPE::IO_RECV;

	send_overlapped.ptr = this;
	send_overlapped.type = IO_TYPE::IO_SEND;
}

CTCPSocket::CTCPSocket(SOCKET _sock)
{
	sock = _sock;

	int addrlen = sizeof(addr);
	getpeername(sock, (SOCKADDR*)&addr, &addrlen);

	recv_overlapped.ptr = this;
	recv_overlapped.type = IO_TYPE::IO_RECV;

	send_overlapped.ptr = this;
	send_overlapped.type = IO_TYPE::IO_SEND;
}

CTCPSocket::~CTCPSocket()
{
	Release();
}

bool CTCPSocket::WSA_Recv(char* buf,  int len)
{
	DWORD recvbyte;
	DWORD flag = 0;
	ZeroMemory(&recv_overlapped.overlapped, sizeof(recv_overlapped.overlapped));

	wsa_recv_buf.buf = buf;
	wsa_recv_buf.len = len;

	int retval = WSARecv(sock, &wsa_recv_buf, 1, &recvbyte, &flag, &recv_overlapped.overlapped, nullptr);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("%d\n", WSAGetLastError());
			CError::GetInstance()->err_display("WSARecv");
			return false;
		}
	}
	return true;
}

bool CTCPSocket::WSA_Send(char* buf, int len)
{
	DWORD sendbyte;
	ZeroMemory(&send_overlapped.overlapped, sizeof(send_overlapped.overlapped));

	wsa_send_buf.buf = buf;
	wsa_send_buf.len = len;

	int retval = WSASend(sock, &wsa_send_buf, 1, &sendbyte, 0, &send_overlapped.overlapped, nullptr);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			CError::GetInstance()->err_display("WSASend");
			return false;
		}
	}
	return true;
}

bool CTCPSocket::WSA_Recv(char* buf, int len, const  
	DWORD _count)
{
	DWORD recvbyte, flag;
	ZeroMemory(&recv_overlapped.overlapped, sizeof(recv_overlapped.overlapped));

	wsa_recv_buf.buf = buf;
	wsa_recv_buf.len = len;

	int retval = WSARecv(sock, &wsa_recv_buf, _count, &recvbyte, &flag, &recv_overlapped.overlapped, nullptr);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			CError::GetInstance()->err_display("WSARecv");
			return false;
		}
	}
	return true;
}

bool CTCPSocket::WSA_Send(char* buf, int len, const DWORD _count)
{
	DWORD sendbyte;
	ZeroMemory(&send_overlapped.overlapped, sizeof(send_overlapped.overlapped));

	wsa_send_buf.buf = buf;
	wsa_send_buf.len = len;

	int retval = WSASend(sock, &wsa_send_buf, _count, &sendbyte, 0, &send_overlapped.overlapped, nullptr);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			CError::GetInstance()->err_display("WSASend");
			return false;
		}
	}
	return true;
}

void CTCPSocket::Release() 
{
	closesocket(sock);
}