#pragma once
#include "State.h"

class Player_State : public CState
{
public:
	Player_State();
	~Player_State() = default;

	unsigned __int64 Get_Sub_State() override;
	void RecvProcess(CClientSection * ptr)override;
	void SendProcess(CClientSection * ptr)override;
private:
	enum class STATE : unsigned __int64
	{
		MOVEMENT = 0x0001000000000000,
		FOCUS = 0x0002000000000000,
		ANIMATION = 0x0003000000000000,
		UPDATE = 0x0004000000000000,
	};


	bool flag = true;
	STATE mState;
};

