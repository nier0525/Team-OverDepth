#pragma once
#include "ClientSection.h"

class CIOCPManager
{
protected:
	HANDLE hcp;
	vector<HANDLE>* hThreadList;
public:
	CIOCPManager();
	~CIOCPManager();

	bool BeginIOCP();
	void RegisterIOCP(SOCKET sock, int key);
	static DWORD WINAPI WorkerThread(LPVOID arg);
	void EndIOCP();

	virtual int Recv(void* ptr, int len) = 0;
	virtual int Send(void* ptr, int len) = 0;
	virtual void Disconneted(void* ptr) = 0;
	virtual void Accept(void* ptr) = 0;
};