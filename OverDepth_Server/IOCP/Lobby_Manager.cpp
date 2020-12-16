#include "Lobby_Manager.h"
IMPLEMENT_SINGLETON(Lobby_Manager)

Lobby_Manager::Lobby_Manager()
{
	lobby_list = nullptr;
	protocol = nullptr;
}

int Lobby_Manager::Packing(char* buf, int result)
{
	char* ptr = buf;
	int size = 0;

	memcpy(ptr, &result, sizeof(result));
	ptr = ptr + sizeof(result);
	size = size + sizeof(result);

	return size;
}

int Lobby_Manager::Packing(char* buf, int result, int lobby_num)
{
	char* ptr = buf;
	int size = 0;

	memcpy(ptr, &result, sizeof(result));
	ptr = ptr + sizeof(result);
	size = size + sizeof(result);

	memcpy(ptr, &lobby_num, sizeof(lobby_num));
	ptr = ptr + sizeof(lobby_num);
	size = size + sizeof(lobby_num);

	return size;
}

int Lobby_Manager::Packing(char* buf, int result, int lobby_num, const char* str)
{
	char* ptr = buf;
	int len = strlen(str);
	int size = 0;

	memcpy(ptr, &result, sizeof(result));
	ptr = ptr + sizeof(result);
	size = size + sizeof(result);

	memcpy(ptr, &lobby_num, sizeof(lobby_num));
	ptr = ptr + sizeof(lobby_num);
	size = size + sizeof(lobby_num);

	memcpy(ptr, &len, sizeof(len));
	ptr = ptr + sizeof(len);
	size = size + sizeof(len);

	memcpy(ptr, str, len);
	ptr = ptr + len;
	size = size + len;

	return size;
}

int Lobby_Manager::Packing(char* buf, int result, const char* str)
{
	char* ptr = buf;
	int len = strlen(str);
	int size = 0;

	memcpy(ptr, &result, sizeof(result));
	ptr = ptr + sizeof(result);
	size = size + sizeof(result);

	memcpy(ptr, &len, sizeof(len));
	ptr = ptr + sizeof(len);
	size = size + sizeof(len);

	memcpy(ptr, str, len);
	ptr = ptr + len;
	size = size + len;

	return size;
}
void Lobby_Manager::Begin()
{
	lobby_list = new vector<Lobby*>();
	lobby_list->push_back(new Lobby());
}

void Lobby_Manager::End()
{
	if (lobby_list != nullptr)
	{
		lobby_list->clear();
		delete lobby_list;
	}
}

bool Lobby_Manager::Add(CClientSection* ptr)
{
	CLock lock;

	bool flag = false;
	char buf[BUFSIZE];
	int size = 0;
	unsigned __int64 full_code = NULL;
	ZeroMemory(buf, sizeof(buf));

	// 현재 매칭하는 클라이언트 플레이어 데이터 생성
	ptr->player_online(new Player());

	for (int i = 0; i < lobby_list->size(); i++)
	{		
		if (!lobby_list->at(i)->Check_Full())//방인원수가 꽉 차지 않았을 경우
		{			
			// 똑같은 유저가 매칭을 두 번 하여 게임이 성립 되는 경우를 방지			
			for (int j = 0; j < lobby_list->at(i)->Get_User_Count(); j++)
			{
				if (lobby_list->at(i)->Get_User_List()->at(j) == ptr)
					return true;
			}

			auto room = lobby_list->at(i);

			// 클라이언트 정보 갱신
			protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::LOBBY_STATE);
			protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());

			room->Add(ptr);

			int num = room->Get_User_Count();
			room->Get_User_List()->at(static_cast<__int64>(num) - 1)->Getplayer()->Set_SerialNum(num);

			// 현재 방정보 갱신
			ptr->Set_RoomInfo(lobby_list->at(i));

			if (room->Get_User_Count() != LOBBY_MAXSIZE) // 인원이 아직 부족한 경우
			{
				printf("NOT FULL\n");
				//size = Packing(buf, lobby_list->size(), (int)RESULT::NOT_READY, GAME_NOT_READY);
				size = Packing(buf, (int)RESULT::NOT_READY, GAME_NOT_READY);		

				ptr->PackingData(full_code, buf, size);
				if (!ptr->Send())
				{
					return false;
				}
			}
			else //인원 꽉 찼을 경우 보내고 바로 나온다.
			{
				printf("FULL\n");

				for (int j = lobby_list->at(i)->Get_User_List()->size() - 1; j >= 0; j--)
				{
					// 이미 게임 중인 대상에게는 매치 완료 패킹을 보낼 필요가 없으므로 예외처리
					if (room->Get_User_List()->at(j)->Get_InGame_State())
						continue;

					size = Packing(buf, (int)RESULT::READY, lobby_list->at(i)->Get_User_List()->at(j)->Getplayer()->Get_SerialNum(), GAME_READY);

					room->Get_User_List()->at(j)->PackingData(full_code, buf, size);
					if (!room->Get_User_List()->at(j)->Send())
					{
						return false;
					}

					room->Get_User_List()->at(j)->Player_InGame();
				}

				for (int j = 0; j < lobby_list->size(); j++)
				{
					if (!lobby_list->at(j)->Check_Full())
						return true;
				}

				printf("새로 생성\n");
				lobby_list->push_back(new Lobby());

				return true;
			}
		}
	}
	return flag;
}

bool Lobby_Manager::Remove(CClientSection* ptr)
{
	CLock lock;

	if (lobby_list->size() == 0)//처음에는 방이 없으므로.
	{
		printf("삭제할 방이 없습니다.\n");
		lobby_list->push_back(new Lobby());
		return false;
	}

	for (int i = 0; i < lobby_list->size(); i++)
	{
		if (lobby_list->at(i)->Remove(ptr))//제거할 인원이 확인 됐을때
		{
			printf("%s Player Offline\n", ptr->GetUser()->name);

			ptr->Player_ExitGame();
			ptr->player_offline();			
			ptr->Set_RoomInfo(nullptr);

			if (lobby_list->at(i)->Get_User_Count() == 0)
			{
				printf("%d Room Empty\n", i + 1);
				lobby_list->erase(lobby_list->begin() + i);
			}
			else
			{				
				char buf[BUFSIZE];
				int size = 0;

				unsigned __int64 full_code = NULL;
				protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::LOBBY_STATE);
				// protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());

				for (int j = 0; j < lobby_list->at(i)->Get_User_Count(); j++)
				{
					ZeroMemory(buf, sizeof(buf));
					CClientSection* ohter = lobby_list->at(i)->Get_User_List()->at(j);
					cout << ohter->GetUser()->name << " Serial Number : " << ohter->Getplayer()->Get_SerialNum() << "→" << j + 1 << endl;

					ohter->Getplayer()->Set_SerialNum(j + 1);

					size = Packing(buf, (int)RESULT::UPDATE_NUMBER, ohter->Getplayer()->Get_SerialNum());
					ohter->PackingData(full_code, buf, size);

					if (!ohter->Send()) return false;
				}
			}
			return true;
		}
	}
	return false;
}

bool Lobby_Manager::ValveComplete(CClientSection* ptr)
{
	CLock lock;

	char buf[BUFSIZE];
	ZeroMemory(buf, sizeof(buf));
	int size = 0;

	const char* data = ptr->UnPackData();

	unsigned __int64 full_code = NULL;
	protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::LOBBY_STATE);

	for (int i = 0; i < lobby_list->size(); i++)
	{
		if (lobby_list->at(i)->Search(ptr))
		{
			int index;
			UnPacking(data, index);

			lobby_list->at(i)->ValveComplete(index);
			if (lobby_list->at(i)->ValveAllComplete())
			{
				size = Packing(buf, (int)RESULT::VALVE_COMPLETE);

				for (int j = 0; j < lobby_list->at(i)->Get_User_Count(); j++)
				{
					CClientSection* player = lobby_list->at(i)->Get_User_List()->at(j);
					player->PackingData(full_code, buf, size);
					if (!player->Send()) return false;
				}
			}
		}
	}
}

void Lobby_Manager::UnPacking(const char* buf, int& index)
{
	const char* ptr = buf;

	memcpy(&index, ptr, sizeof(int));
}



