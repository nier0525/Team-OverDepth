#pragma once
#include "State.h"
#include"Lobby_Manager.h"

class Lobby_State : public CState
{
public:
	Lobby_State();
	~Lobby_State() = default;

	unsigned __int64 Get_Sub_State() override;
	void RecvProcess(CClientSection* ptr)override;
	void SendProcess(CClientSection* ptr)override;
private:
	enum class STATE : unsigned __int64
	{
		NONE = 0x0000000000000000,
		ENTER = 0x0001000000000000,
		LEAVE = 0x0002000000000000,
		VALVE = 0x0003000000000000
	};

	STATE mState;
};

