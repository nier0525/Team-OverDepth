#pragma once
#include "State.h"
#include "LoginManager.h"

class CLoginState : public CState
{
private:
	enum class STATE : unsigned __int64
	{
		MENU_SELECT_STATE = 0x0001000000000000,
		SEND_RESULT_STATE = 0x0002000000000000
	};

	STATE mState;
public:
	CLoginState();
	~CLoginState();

	unsigned __int64 Get_Sub_State() override;
	void RecvProcess(CClientSection* ptr)override;
	void SendProcess(CClientSection* ptr)override;
};