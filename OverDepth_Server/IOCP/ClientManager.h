#pragma once
#include "CriticalSection.h"
#include "ClientSection.h"

class ClientManager : public CMultiThreadSyns<ClientManager>
{
private:
	DECLARE_SINGLETONE(ClientManager)

	ClientManager();
	~ClientManager();
private:
	list<CClientSection*>* mClient_List;
	CClientSection* mClient_Data;
public:
	void Begin();
	void End();

	CClientSection* AddClient(SOCKET _sock);
	void RemoveClient(CClientSection* ptr);

	CClientSection* SearchClientInfo(const char* id);
	CClientSection* SearchClientData();

	//void SearchClientBegin();
	//void SearchClientEnd();
};