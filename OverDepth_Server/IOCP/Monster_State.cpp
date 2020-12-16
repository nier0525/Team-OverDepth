#include "Monster_State.h"
#include "MonsterManager.h"

Monster_State::Monster_State()
{

}

unsigned __int64 Monster_State::Get_Sub_State()
{
	return (unsigned __int64)mState;
}

void Monster_State::RecvProcess(CClientSection* ptr)
{
	if (ptr->Get_RoomInfo() == nullptr)
	{
		printf("[Monster Recv] Room Empty\n");
		return;
	}

	unsigned __int64 full_code = ptr->GetProtocol();
	//cout << "Player State" << hex << full_code << endl;

	bool code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, (unsigned __int64)CLASS_STATE::MONSTER_STATE, NULL, NULL);

	if (code_check)
	{
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::MERMAN, NULL);
		if (code_check)
		{
			MonsterManager::GetInstance()->MemenUpdateState(ptr, full_code, (unsigned __int64)STATE::MERMAN);
			mState = STATE::MERMAN;
		}

		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::FANATIC_SWORD, NULL);
		if (code_check)
		{
			MonsterManager::GetInstance()->FanaticSwordUpdateState(ptr, full_code, (unsigned __int64)STATE::FANATIC_SWORD);
			mState = STATE::FANATIC_SWORD;
		}

		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::FANATIC_MASE, NULL);
		if (code_check)
		{
			MonsterManager::GetInstance()->FanaticMaseUpdateState(ptr, full_code, (unsigned __int64)STATE::FANATIC_MASE);
			mState = STATE::FANATIC_MASE;
		}

		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::FANATIC_GUN, NULL);
		if (code_check)
		{
			MonsterManager::GetInstance()->FanaticGunUpdateState(ptr, full_code, (unsigned __int64)STATE::FANATIC_GUN);
			mState = STATE::FANATIC_GUN;
		}

		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL, (unsigned __int64)STATE::TITANICHYDRA, NULL);
		if (code_check)
		{
			MonsterManager::GetInstance()->TitanicHydraUpdateState(ptr, full_code, (unsigned __int64)STATE::TITANICHYDRA);
			mState = STATE::TITANICHYDRA;
		}
	}
}


void Monster_State::SendProcess(CClientSection* ptr)
{

}