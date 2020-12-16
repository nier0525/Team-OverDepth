#pragma once
#include "Player.h"
#include"Lobby.h"
#include "CProtocol.h"

class Lobby_Manager : public CMultiThreadSyns<Lobby_Manager>
{
public:	
	void Begin();
	void End();
	bool Add(CClientSection* ptr);
	bool Remove(CClientSection* ptr);
	bool ValveComplete(CClientSection* ptr);
private:
	Lobby_Manager();
	~Lobby_Manager() = default;
	//이부분 수정
	enum class RESULT
	{
		NOT_READY,
		READY,
		UPDATE_NUMBER,
		VALVE_COMPLETE,
	};

	int Packing(char* buf, int result);
	int Packing(char* buf, int result, int lobby_num);
	int Packing(char* buf, int result, int lobby_num, const char* str);
	int Packing(char* buf, int result, const char* str);

	void UnPacking(const char* buf, int& index);
private:
	DECLARE_SINGLETONE(Lobby_Manager)
	CProtocol* protocol;
	vector<Lobby*>* lobby_list;
};

