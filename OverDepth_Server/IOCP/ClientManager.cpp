#include "ClientManager.h"
IMPLEMENT_SINGLETON(ClientManager)

ClientManager::ClientManager()
{
	mClient_List = nullptr;
	mClient_Data = nullptr;
}

ClientManager::~ClientManager()
{

}

void ClientManager::Begin()
{
	mClient_List = new list<CClientSection*>();
}

void ClientManager::End()
{
	if (mClient_List != nullptr) delete mClient_List;
	if (mClient_Data != nullptr) delete mClient_Data;
}

CClientSection* ClientManager::AddClient(SOCKET sock)
{
	CLock lock;

	CClientSection* client = new CClientSection(sock);
	mClient_List->push_back(client);

	printf("클라이언트 접속 : IP : %s , PORT : %d\n\n", inet_ntoa(client->GetAddr().sin_addr), ntohs(client->GetAddr().sin_port));
	return client;
}

void ClientManager::RemoveClient(CClientSection* ptr)
{
	CLock lock;

	printf("클라이언트 종료 : IP : %s , PORT : %d\n\n", inet_ntoa(ptr->GetAddr().sin_addr), ntohs(ptr->GetAddr().sin_port));
	if (ptr->IsLogin())
		ptr->Logout();
	mClient_List->remove(ptr);
	delete ptr;
}

CClientSection* ClientManager::SearchClientInfo(const char* id)
{
	CLock lock;

	mClient_Data = nullptr;
	
	for (list<CClientSection*>::iterator data = mClient_List->begin(); data != mClient_List->end(); data++)
	{
		mClient_Data = *data;

		if (!strcmp(mClient_Data->GetUser()->id, id))
		{
			break;
		}
	}

	return mClient_Data;
}

CClientSection* ClientManager::SearchClientData()
{
	return mClient_Data;
}

//void ClientManager::SearchClientBegin()
//{
//	mClient_List->begin();
//}
//
//void ClientManager::SearchClientEnd()
//{
//	mClient_List->end();
//}
