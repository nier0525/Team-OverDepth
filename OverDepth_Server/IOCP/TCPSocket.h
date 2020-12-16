#pragma once
#include "Error.h"
#include "CriticalSection.h"

class CTCPSocket : public CMultiThreadSyns<CTCPSocket>
{
protected:
	WSAOverLapped_EX recv_overlapped;
	WSAOverLapped_EX send_overlapped;

	SOCKET sock;
	SOCKADDR_IN addr;

	WSABUF wsa_recv_buf;
	WSABUF wsa_send_buf;

public:
	CTCPSocket();
	CTCPSocket(SOCKET _sock);
	~CTCPSocket();

	bool WSA_Recv(char* buf, int len);
	bool WSA_Send(char* buf, int len);

	bool WSA_Recv(char* buf, int len, const DWORD _count);
	bool WSA_Send(char* buf, int len, const DWORD _count);

	SOCKET GetSocket() { return sock; }
	SOCKADDR_IN GetAddr() { return addr; }

	void Release();
};