#pragma once
#pragma warning (disable : 4996)

#include <WinSock2.h>
#pragma comment (lib, "ws2_32")

#include <mysql.h>
#pragma comment (lib, "libmysql.lib")

#include <stdio.h>
#include <stdlib.h>

#define PORT 9050
#define BUFSIZE 4096
#define MSGSIZE 128

#define LOBBY_MAXSIZE 2

#include <time.h>

#include <vector>
#include <list>
#include <queue>
#include<iostream>
using namespace std;

#define DB_HOST "localhost"	// DB IP주소 또는 도메인 또는 localhost
#define DB_USER "root"		// DB접속 계정 명
#define DB_PASS "1234"		// DB접속 계정 암호
#define DB_NAME "userdb"	// DB 이름

#define JOIN_SUCCESS_MSG "Join Success\n"
#define ERROR_JOIN_MSG "Join Error\n"
#define LOGIN_SUCCESS_MSG "Login Success\n"
#define ERROR_LOGIN_ID_MSG "ID Error\n"
#define ERROR_LOGIN_PW_MSG "PW Error\n"
#define ERROR_LOGIN_EXISTS_MSG "Exists Error\n"

#define GAME_NOT_READY "Wait for other player"
#define GAME_READY "GAME START"

enum class CLASS_STATE : unsigned __int64
{ //OB 는 이진수 표현
  //숫자 앞에 0 붙이면 8진수 
  //숫자 앞에 0X 붙이면 16진수
	NO_STATE =   0x0000000000000000,
	INIT_STATE =  0x0100000000000000,
	LOGIN_STATE = 0x0200000000000000,
	CHAT_STATE =  0x0300000000000000,
	LOBBY_STATE = 0x0400000000000000,
	PLAYER_STATE = 0x0500000000000000,
    MONSTER_STATE = 0x0600000000000000,

};

enum IO_TYPE
{
	IO_RECV = 1,
	IO_SEND,
	IO_ACCEPT,
};

enum
{
	SOC_ERROR = -1,
	SOC_TRUE,
	SOC_FALSE,
};

enum UserInfo_Data
{
	ID = 0,
	PW,
	NAME
};

struct WSAOverLapped_EX
{
	WSAOVERLAPPED overlapped;
	LPVOID ptr;
	IO_TYPE type;
};

struct UserInfo
{
	char id[MSGSIZE];
	char pw[MSGSIZE];
	char name[MSGSIZE];

	int number;
	bool login;

	UserInfo() 
	{
		memset(this, 0, sizeof(*this));
		login = false;
	}
	UserInfo(char* _id, char* _pw, char* _name, int numb)
	{
		strcpy(id, _id);
		strcpy(pw, _pw);
		strcpy(name, _name);
		number = numb;
		login = false;
	}
};

// 각 위치,회전 정보만 담을 구조체
class Vector3
{
public:
    float x, y, z;

    Vector3()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    Vector3(const Vector3& vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }

    Vector3(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
};

class Quaternion
{
public:
    float x, y, z, w;

    Quaternion()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }

    Quaternion(const Quaternion& rot)
    {
        x = rot.x;
        y = rot.y;
        z = rot.z;
        w = rot.w;
    }

    Quaternion(float _x, float _y, float _z, float _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }
};


// 싱글톤 매크로 ------------------------------------------------------------------

#define MAKE_NO_COPY(CLASSNAME)                                             \
        private:                                                            \
               CLASSNAME(const CLASSNAME&){}                                \
               CLASSNAME& operator=(const CLASSNAME&);

// 싱클톤 패턴 생성 매크로
#define DECLARE_SINGLETONE(CLASSNAME)                                       \
        MAKE_NO_COPY(CLASSNAME)                                             \
        private:                                                            \
			   static CLASSNAME* mPthis;									\
        public:                                                             \
               static CLASSNAME* Create();									\
			   static CLASSNAME* GetInstance();								\
               static void Destroy();										 

// 싱글톤 패턴 구현 매크로

#define IMPLEMENT_SINGLETON(CLASSNAME)                              \
               CLASSNAME* CLASSNAME::mPthis= NULL;					\
                                                                    \
               CLASSNAME* CLASSNAME::Create()						\
			   {													\
                       if(mPthis == NULL)							\
						{											\
                              mPthis=new CLASSNAME();				\
						}											\
                        return mPthis;								\
               }                                                    \
               CLASSNAME* CLASSNAME::GetInstance()					\
               {                                                    \
                    return mPthis;									\
               }													\
			   void CLASSNAME::Destroy()							\
               {													\
					if(mPthis) delete mPthis;						\
			   }																