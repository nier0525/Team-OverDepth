#include "Player_State.h"
#include"Player_Manager.h"

Player_State::Player_State()
{
	// mState = STATE::NONE;
}

unsigned __int64 Player_State::Get_Sub_State()
{
	return (unsigned __int64)mState;
}

void Player_State::RecvProcess(CClientSection* ptr)
{
	unsigned __int64 full_code = ptr->GetProtocol();
	//cout << "Player State" << hex << full_code << endl;

	bool code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE, NULL, NULL);

	if (code_check)
	{
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::MOVEMENT, NULL);
		if (code_check)
		{
			PlayerManager::GetInstance()->PlayerMovement(ptr, full_code, (unsigned __int64)STATE::MOVEMENT);
		}

		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::FOCUS, NULL);
		if (code_check)
		{
			PlayerManager::GetInstance()->PlayerFocus(ptr, full_code, (unsigned __int64)STATE::FOCUS);
		}

		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::UPDATE, NULL);
		if (code_check)
		{
			PlayerManager::GetInstance()->PlayerUpdate(ptr, full_code, (unsigned __int64)STATE::UPDATE);
		}

		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::ANIMATION, NULL);
		if (code_check)
		{
			PlayerManager::GetInstance()->PlayerAnimation(ptr, full_code, (unsigned __int64)STATE::ANIMATION);
		}
	}

	/*
	flag = true;
	unsigned __int64 full_code = ptr->GetProtocol();
	cout << "Player_State" << hex << full_code << endl;
	//이 부분 수정 클라이언트의 main state 확인
	bool code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, (unsigned __int64)CLASS_STATE::PLAYER_STATE, NULL, NULL);

	if (code_check == TRUE)
	{
		//이 부분 Lobby Manager에서 바로 해줌
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::ONLINE, NULL);
		if (code_check == TRUE)
		{
			printf("ONLINE\n");
			mState = STATE::ONLINE;
			Player_Manager::GetInstance()->Send_Into_Game(ptr);
			return;
		}
		//이 부분 Lobby Manager에서 바로 해줌
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::MOVEMENT, NULL);
		if (code_check == TRUE)
		{
			printf("MOVEMENT\n");
			mState = STATE::MOVEMENT;
			if (!Player_Manager::GetInstance()->Player_Movement(ptr, full_code))
			{
				flag = false;
			}
			return;
		}
		//이 부분 수정 클라이언트의 sub state 확인
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::OFFLINE, NULL);
		if (code_check == TRUE)
		{
			printf("OFFLINE\n");
			mState = STATE::OFFLINE;
			Player_Manager::GetInstance()->Remove_From_Game(ptr);
			return;
		}
	}
	else
	{
		return;
	}
	*/
}

void Player_State::SendProcess(CClientSection* ptr)
{
	/*
	//player send가 남았을 경우 더 보내는 코드 수종
	switch (mState)
	{
	case Player_State::STATE::NONE:
		break;
	case Player_State::STATE::ONLINE:
		break;
	case Player_State::STATE::MOVEMENT:
		break;
	case Player_State::STATE::OFFLINE:
		break;
	}
	*/
	return;
}
