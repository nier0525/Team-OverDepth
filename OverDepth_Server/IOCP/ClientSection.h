#pragma once
#include "Packing.h"
#include "State.h"
#include "Player.h"
#include "Lobby.h"

class Lobby;

class CInitState;
class CLoginState;
class ChatState;
class Lobby_State;
class Player_State;
class Monster_State;

class CClientSection : public CPacking
{
protected:
	// ETC
	UserInfo* user;
	bool login_state;

	Player* player;
	Lobby* lobby_info;
	bool InGame;

	// State
	CState* mState;

	CInitState* mInitState;
	CLoginState* mLoginState;
	ChatState* mChatState;
	Lobby_State* mLobbyState;
	Player_State* mPlayerState;
	Monster_State* mMonsterState;
public:
	CClientSection();
	CClientSection(SOCKET _sock);
	~CClientSection();

	// State
	void SetState(CState* state);
	CState* GetState();

	CInitState* GetinitState();
	CLoginState* GetLoginState();
	ChatState* GetChatState();
	Lobby_State* GetLobbyState();
	Player_State* GetPlayerState();
	Monster_State* GetMonaterState();

	// ETC
	void Login(UserInfo* _user);
	void Logout();
	
	UserInfo* GetUser() { return user; }
	bool IsLogin() { return login_state; }

	void player_online(Player* _player);
	void player_offline();
	Player* Getplayer() { return player; }

	void Set_RoomInfo(Lobby* lobby) { lobby_info = lobby; }
	Lobby* Get_RoomInfo() { return lobby_info; }

	void Player_InGame() { InGame = true; }
	void Player_ExitGame() { InGame = false; }

	bool Get_InGame_State() { return InGame; }
};