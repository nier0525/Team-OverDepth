#pragma once
#include "CProtocol.h"
#include "ClientSection.h"

class ChatManager : public CMultiThreadSyns<ChatManager>
{
private:
	DECLARE_SINGLETONE(ChatManager)
	CProtocol* protocol;
	list< CClientSection*>* chat_user;
	ChatManager();
	~ChatManager() = default;
private:
	void UnPacking(const char* buf, char* msg);
	int Packing(char* buf, const char* str);
public:
	//이부분 수정
	enum class PROTOCOL : unsigned __int64
	{
		SEND = 0x0000010000000000,
		RECV = 0x0000020000000000
	};
	void Begin();
	void End();

	void Add(CClientSection* ptr);
	void Remove(CClientSection* ptr);

	bool Recv_Message(CClientSection* ptr);
	bool Send_Message(CClientSection* ptr);
};

