#pragma once
#include "CriticalSection.h"
#include "MySQLDataBaseManager.h"
#include "ListenSocket.h"
#include "IOCPManager.h"
#include "CProtocol.h"

#include "ClientManager.h"
#include "LoginManager.h"
#include "ChatManager.h"
#include "Lobby_Manager.h"
#include "Player_Manager.h"
#include "MonsterManager.h"

class CServerManager : public CIOCPManager
{
private:
	static CServerManager* pthis;
	vector<unsigned __int64> class_state;
	CServerManager();
	~CServerManager();

	CListenSocket* mListenSock;
public:
	static CServerManager* GetInstance();
	static void Destory();

	bool Begin();
	void Run();
	void End();

	int Recv(void* ptr, int len);
	int Send(void* ptr, int len);
	void Disconneted(void* ptr);
	void Accept(void* ptr);
};