#pragma once
#include "Sector.h"

char Map[18][21] = {
	"####################",
	"####################",
	"####################",
	"#####   ############",
	"#####O  ############",
	"#####  O############",
	"###  O@  ###########",
	"### # ## ###########",
	"#   # ## #####  ..##",
	"# O             ..##",
	"##### ### # ##  ..##",
	"#####     ##########",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################",
	"####################"
};

class CSectorManager
{
private:
	vector< vector<CSector*>* >* vSector;
public:
	CSectorManager()
	{
		vSector = nullptr;
	}
	~CSectorManager()
	{
		if (vSector != nullptr) delete vSector;
	}

	void Begin()
	{
		vSector = new vector< vector<CSector*>* >();
		vSector->assign(18, new vector<CSector*>(21, nullptr));

		int id = 1;

		for (int i = 0; i < 18; i++)
		{
			vector<CSector*>* v = new vector<CSector*>();

			for (int j = 0; j < 21; j++)
			{
				CSector* sector = new CSector(id++);
				v->push_back(sector);
			}

			vSector->at(i) = v;
		}

		for (int y = 0; y < 18; y++)
		{
			for (int x = 0; x < 21; x++)
			{
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						int check_x = x + j;
						int check_y = y + i;

						if (check_x >= 0 && check_x < 21 && check_y >= 0 && check_y < 18)
						{
							vSector->at(y)->at(x)->Insert_AlmostSector(vSector->at(check_y)->at(check_x));
						}
					}
				}
			}
		}

		BeginSector();
	}

	void End()
	{
		vSector->clear();
	}

	void BeginSector()
	{
		for (int y = 0; y < 18; y++)
		{
			for (int x = 0; x < 21; x++)
			{
				if (Map[y][x] == '@' || Map[y][x] == 'O')
				{
					vSector->at(y)->at(x)->OnEnter(Map[y][x]);

					for (auto data = vSector->at(y)->at(x)->GetAlmostSectorList()->begin(); data != vSector->at(y)->at(x)->GetAlmostSectorList()->end(); data++)
					{
						CSector* sector = *data;
						// sector->Send();

						printf("%d Send\n", sector->GetID());
					}
					printf("\n");
				}
			}
		}
	}

	void UpdateSector(/*CClientSection* ptr*/)
	{
		for (int y = 0; y < 18; y++)
		{
			for (int x = 0; x < 21; x++)
			{
				CSector* sector = vSector->at(y)->at(x);

				if (sector->GetChar() == '@')
				{
					for (auto data = sector->GetAlmostSectorList()->begin(); data != sector->GetAlmostSectorList()->end(); data++)
					{
						CSector* almost = *data;

						if (almost->GetChar() == '@')
						{
							// almost->Send();
							printf("Client Sector ID : %d\n", almost->GetID());
						}

						if (almost->GetChar() == 'O')
						{
							printf("Object Sector ID : %d\n", almost->GetID());
						}
					}

					printf("\n");
				}
			}
		}

		/*
		int x , y;

		// ID 로 셉터 찾기
		for (int i = 0; i < 18; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				if (vSector->at(i)->at(j)->GetID() == ptr->GetID())
				{
					x = i;
					y = j;
					break;
				}
			}
		}

		// 플레이어가 이동 했을 경우
		if (GetProtocol() == MOVE)
		{
			MoveSector(ptr, x, y);
		}

		// 인접 셉터의 클라이언트 리스트에게 데이터 전송
		for (auto data = vSector->at(y)->at(x)->GetAlmostSectorList()->begin(); data != vSector->at(y)->at(x)->GetAlmostSectorList()->end(); data++)
		{
			CSector* sector = *data;
			// sector->Send();

			printf("%d Send\n", sector->GetID());
		}
		*/
	}

	void MoveSector(/*CClientSection* ptr, int& _x, int& _y*/)
	{
		/*
		// 현재 플레이어가 자신의 셉터 영역을 이탈하고 다른 셉터 영역에 들어갔는 지 확인
		if (. . .)
		{
			int pre_id = vSector->at(_y)->at(_x)->GetID();

			int x = _x;
			int y = _y;

			// 어느 방향으로 이동했는 지 확인 후, x,y 변수에 위치 저장
			if ()
			else if ()
			. . .

			vSector->at(_y)->at(_x)->OnExit(ptr);
			vSector->at(y)->at(x)->OnEnter(ptr);

			// 이전 셉터의 인급 셉터들에게 데이터 전송
			for (auto data = vSector->at(y)->at(x)->GetAlmostSectorList()->begin(); data != vSector->at(y)->at(x)->GetAlmostSectorList()->end(); data++)
			{
				CSector* sector = *data;
				// sector->Send();

				printf("%d Send\n", sector->GetID());
			}

			_x = x;
			_y = y;
		}
		else
			return;
		*/
	}
};
