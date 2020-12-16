#include "Lobby_State.h"

Lobby_State::Lobby_State()
{
	mState = STATE::NONE;
}

unsigned __int64 Lobby_State::Get_Sub_State()
{
	return (unsigned __int64)mState;
}

void Lobby_State::RecvProcess(CClientSection* ptr)
{
	unsigned __int64 full_code = ptr->GetProtocol();
	cout << "Lobby_State" << hex << full_code << endl;

	//이 부분 수정 클라이언트의 main state 확인
	bool code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, (unsigned __int64)CLASS_STATE::LOBBY_STATE, NULL, NULL);
	if (code_check == TRUE)
	{
		//이 부분 수정 클라이언트의 sub state 확인
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::ENTER, NULL);
		if (code_check == TRUE)
		{
			printf("ENTER\n");
			mState = STATE::ENTER;
			Lobby_Manager::GetInstance()->Add(ptr);
			return;
		}
		//이 부분 수정 클라이언트의 sub state 확인
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::LEAVE, NULL);
		if (code_check == TRUE)
		{
			printf("Remove\n");
			mState = STATE::LEAVE;
			Lobby_Manager::GetInstance()->Remove(ptr);
			return;
		}

		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::VALVE, NULL);
		if (code_check == TRUE)
		{
			printf("VALVE\n");
			mState = STATE::VALVE;
			Lobby_Manager::GetInstance()->ValveComplete(ptr);
			return;
		}
	}
	else
	{
		return;
	}
}

void Lobby_State::SendProcess(CClientSection* ptr)
{
	return;
}
