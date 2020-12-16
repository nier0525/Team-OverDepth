#pragma once
#include "State.h"

class Monster_State : public CState
{
public:
	Monster_State();
	~Monster_State() = default;

	unsigned __int64 Get_Sub_State() override;
	void RecvProcess(CClientSection* ptr)override;
	void SendProcess(CClientSection* ptr)override;
private:
	enum class STATE : unsigned __int64
	{
		MERMAN = 0x0001000000000000,
		FANATIC_SWORD = 0x0002000000000000,
		FANATIC_MASE = 0x0003000000000000,
		FANATIC_GUN = 0x0004000000000000,
		TITANICHYDRA = 0x0005000000000000,
	};


	bool flag = true;
	STATE mState;
};

