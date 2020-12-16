#include "IOCPManager.h"

CIOCPManager::CIOCPManager()
{
	hcp = nullptr;
	hThreadList = nullptr;
}

CIOCPManager::~CIOCPManager()
{
	if (hThreadList != nullptr)
		delete hThreadList;
}

bool CIOCPManager::BeginIOCP()
{
	hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if (hcp == nullptr) return false;

	SYSTEM_INFO sys;
	GetSystemInfo(&sys);

	hThreadList = new vector<HANDLE>();

	for (int i = 0; i < (int)sys.dwNumberOfProcessors * 2; i++)
	{
		hThreadList->push_back(CreateThread(NULL, 0, WorkerThread, this, 0, NULL));
	}
}

DWORD WINAPI CIOCPManager::WorkerThread(LPVOID arg)
{
	int retval;
	CIOCPManager* ptr = (CIOCPManager*)arg;

	while (1)
	{
		DWORD cbTransferrd;
		SOCKET sock = NULL;
		WSAOverLapped_EX* overlapped;

		retval = GetQueuedCompletionStatus(ptr->hcp, &cbTransferrd, (PULONG_PTR)&sock, (LPOVERLAPPED*)&overlapped, INFINITE);

		if (sock == NULL && (retval == 0 || cbTransferrd == 0))
		{
			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(sock, &overlapped->overlapped, &temp1, false, &temp2);
				CError::GetInstance()->err_display("WSAGetOverlappedResult");
			}

			ptr->Disconneted(overlapped->ptr);
			continue;
		}

		switch (overlapped->type)
		{
		case IO_RECV:
		{
			int recv = ptr->Recv(overlapped->ptr, cbTransferrd);
			switch (recv)
			{
			case SOC_ERROR:
				ptr->Disconneted(overlapped->ptr);
			case SOC_FALSE:
				continue;
			}
		}
			break;

		case IO_SEND:
		{
			int send = ptr->Send(overlapped->ptr, cbTransferrd);
			switch (send)
			{
			case SOC_ERROR:
				ptr->Disconneted(overlapped->ptr);
			case SOC_FALSE:
				continue;
			}
		}
			break;

		case IO_ACCEPT:
			ptr->Accept(overlapped);
			break;
		}
	}

	return 0;
}

void CIOCPManager::RegisterIOCP(SOCKET sock, int key)
{
	CreateIoCompletionPort((HANDLE)sock, hcp, key, 0);
}

void CIOCPManager::EndIOCP()
{
	hcp = nullptr;
	hThreadList->clear();
}