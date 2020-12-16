#pragma once
#include "State.h"
#include"ChatManager.h"


// RPOTOCOL Çü½Ä
// 1. 
class ChatState  :public CState
{
private:
	enum class STATE : unsigned __int64
	{
		RECV_STATE = 0x0001000000000000,
		SEND_STATE = 0x0002000000000000
	};

	STATE mState;
public:
	ChatState();
	~ChatState() = default;
	unsigned __int64 Get_Sub_State() override;
	void RecvProcess(CClientSection* ptr)override;
	void SendProcess(CClientSection* ptr)override;
};

