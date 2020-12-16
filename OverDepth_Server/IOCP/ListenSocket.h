#pragma once
#include "Error.h"

class CListenSocket
{
private:
	SOCKET sock;
public:
	CListenSocket();
	~CListenSocket();

	void TCP_Setting(const char* _id, const short _port);
	void Listen();
	void Listen(const int _count);
	SOCKET Accept();

	SOCKET GetListenSocket() { return sock; }

	void Release();
};