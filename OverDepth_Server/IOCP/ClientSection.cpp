#include "ClientSection.h"
#include "InitState.h"
#include "LoginState.h"
#include "ChatState.h"
#include "Lobby_State.h"
#include "Player_State.h"
#include "Monster_State.h"

CClientSection::CClientSection() 
{
	user = nullptr;
	login_state = false;

	player = nullptr;
	lobby_info = nullptr;
	InGame = false;

	mInitState = new CInitState();
	mLoginState = new CLoginState();
	mChatState = new ChatState();
	mLobbyState = new Lobby_State();
	mPlayerState = new Player_State();
	mMonsterState = new Monster_State();

	mState = mInitState;
}

CClientSection::CClientSection(SOCKET _sock) : CPacking(_sock)
{
	user = nullptr;
	login_state = false;
	
	player = nullptr;
	lobby_info = nullptr;
	InGame = false;

	mInitState = new CInitState();
	mLoginState = new CLoginState();
	mChatState = new ChatState();
	mLobbyState = new Lobby_State();
	mPlayerState = new Player_State();
	mMonsterState = new Monster_State();

	mState = mInitState;
}

CClientSection::~CClientSection() 
{
	if (user != nullptr) delete user;

	if (player != nullptr) delete player;

	if (mInitState != nullptr) delete mInitState;

	if (mLoginState != nullptr) delete mLoginState;
}

void CClientSection::SetState(CState* state)
{
	mState = state;
}
CState* CClientSection::GetState()
{
	return mState; 
}

CInitState* CClientSection::GetinitState() 
{
	return mInitState; 
}

CLoginState* CClientSection::GetLoginState()
{ 
	return mLoginState;
}

ChatState* CClientSection::GetChatState()
{ 
	return mChatState;
}

Lobby_State* CClientSection::GetLobbyState()
{
	return this->mLobbyState;
}

Player_State* CClientSection::GetPlayerState()
{
	return this->mPlayerState;
}

Monster_State* CClientSection::GetMonaterState()
{
	return this->mMonsterState;
}



void CClientSection::Login(UserInfo* _user)
{
	CLock lock;

	user = _user;
	user->login = true;
	login_state = true;
}

void CClientSection::Logout()
{
	CLock lock;

	user->login = false;
	user = nullptr;
	login_state = false;
}

void CClientSection::player_online(Player* _player)
{
	CLock clock;
	player = _player;
}

void CClientSection::player_offline()
{
	CLock clock;
	if (player != nullptr)
		delete player;

	player = nullptr;
}
