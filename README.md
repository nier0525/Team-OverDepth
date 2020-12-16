# Team-OverDepth
--------------------------------------------
팀명 : OverDepth  
게임명 : OverDepth  
개발 기간 : 2020.03.01 - 2020.12.03  
개발 환경 : Unity3D / Unity C# / C++ / Github Desktop / Trello  
개발 인원 : 11명 ( 기획 3 / 프로그래밍 4 / 그래픽 4 )  
  
```  
최대 2인까지 협동 가능한 3D 멀티 액션 게임으로 OverDepth 만의 특수한 기믹과 강력한 몬스터들을 협력하여 공략해 나가는 게임입니다.  
G-Star 2020 출품, 교내 경진대회 출품 및 최우수상 수상, GGC ( Global Game Challenge 2020 ) 출품 등의 성과를 내었습니다.  
```  
  
+ 담당 업무 : 서버 / 클라이언트 프로그래머  
  + 서버 세부 업무 : 라이브러리 설계, MYSQL 연동, 본 게임과의 연동 등 전체적인 모든 서버 시스템  
  + 클라이언트 세부 엄무 : 게임 UI, 옵션, 타이틀 화면과 같은 게임 시스템 기능 및 플레이어 서버 동기화와 보간 기능  
  
  
# 게임 소개
-------------------------------------------  
  
게임 영상 링크 : https://www.youtube.com/watch?v=y0tODYMJPVg  
  
<img src = "https://user-images.githubusercontent.com/63215359/102127979-86a36500-3e90-11eb-8acd-45a96c9f0246.PNG" width="350" height="300">
<img src = "https://user-images.githubusercontent.com/63215359/102128119-bc484e00-3e90-11eb-9ab5-afeb920b8e88.PNG" width="350" height="300">
<img src = "https://user-images.githubusercontent.com/63215359/102128137-c23e2f00-3e90-11eb-8afb-220d5221c968.PNG" width="350" height="300">
  
```  
OverDepth 는 최대 2인 협동이 가능한 3D 멀티 액션 게임 입니다.  
Soul Like 류의 게임 중 대표격인 블러드본, 다크소울과 같은 게임에 영향이 많이 받았습니다.  
  
게임 시작 시 플레이 할 캐릭터를 선택하고 다른 유저와의 매칭을 대기하게 됩니다.  
2인이 서로 매칭이 되었을 경우 게임을 진행할 수 있으며 서로 협동하여 각종 퍼즐과 몬스터를 공략하고,  
최종적으로 마지막 보스를 제거하면 게임을 클리어 할 수 있습니다.  
```  

# 담당 게임 기능
------------------------------------------  
  
+ 서버  
  + 서버 라이브러리  
  + 클라이언트 라이브러리  
  + 플레이어 동기화  
  + 서버-클라이언트 보간  
+ 클라이언트  
  + 각종 유저 편의 기능  
    
## 서버
-----------------------------------------
### 서버 라이브러리
-----------------------------------------
  
서버 라이브러리는 IOCP 를 기반으로 설계되었으며 DB 는 MYSQL 과 연동하고 있습니다.  
  
#### 1. 프로토콜
--------------------------------------------------------------  
  
프로토콜은 바이트 연산을 이용하여 그 기능과 역할을 세분화하여 관리하기 용이하도록 구현하였고, 기본적으로 Main, Sub, Protocol 로 나누어 사용하고 있습니다.  
  
Main 의 역할은 그 기능의 큰 틀로 보통 클래스명을 사용하고 합니다. ( MonsterClass, PlayerClass . . . )  
Sub 의 역할은 클래스 내에 어떤 멤버 함수를 사용할 것인가에 대한 기능으로 그 기준은 사용자의 재량에 따릅니다.  
( Attack, Move 또는 몬스터 종류별 함수 등 . . . )  
Protocol 의 역할은 어떤 기능을 사용할 것인지를 선택하는 기능으로 가장 핵심적인 역할을 담당하고 있습니다.
  
#### 헤더 
```  
#pragma once
#include"Global.h"

class CProtocol
{
private:
    //1$3 = 일치
    //2&6 = 일치
    //2&7 = 일치 
    //& 연산을 하기 때문에 1이 아닌것은 다 0으로 처리 하기 때문.
    //1, 3, 7, f, 3f, 7f, ff
    DECLARE_SINGLETONE(CProtocol);
    CProtocol() = default;
    ~CProtocol() = default;
    enum class FULL_CODE :unsigned __int64
    {
        MAIN = 0xff00000000000000,
        SUB =  0x00ff000000000000,
        PROTOCOL = 0x0000ff0000000000
    };
public:
    void ProtocolMaker(unsigned __int64& Full_Code, unsigned __int64 input);
    bool ProtocolUnpacker(unsigned __int64 _full_code, unsigned __int64 main, unsigned __int64 sub, unsigned __int64 protocol);
};
```  
  
#### 구현부
  
```  
#include "CProtocol.h"
IMPLEMENT_SINGLETON(CProtocol)

void CProtocol::ProtocolMaker(unsigned __int64& Full_Code, unsigned __int64 input)
{
    Full_Code = Full_Code | input;
}

bool CProtocol::ProtocolUnpacker(unsigned __int64 _full_code, unsigned __int64 main, unsigned __int64 sub, unsigned __int64 protocol)
{
    unsigned __int64 temp = 0;
    int variable_count = 0;//입력받은 매개변수 개수
    if (main != NULL)
    {
        temp = _full_code & (unsigned __int64)FULL_CODE::MAIN;
        variable_count++;
    }
    if (sub != NULL)
    {
        temp = _full_code & (unsigned __int64)FULL_CODE::SUB;
        variable_count++;
    }
    if (protocol != NULL)
    {
        temp = _full_code & (unsigned __int64)FULL_CODE::PROTOCOL;
        variable_count++;
    }
    if (variable_count == 2)
    {
        printf("입력 가능 매개변수의 수를 초과하셨습니다.");
        return false;
    }
    if (temp == main|| temp == sub || temp == protocol)
    {
        return true;
    }
    else
    {
        return false;
    }

}
```  
  
#### 2. MYSQL 연동
--------------------------------------------------------------  
  
DB 는 MYSQL 을 사용하여 관리하도록 구현 하였고, 본 게임에서는 유저의 계정 정보 정도만 관리하고 있습니다.  
구현된 기능은 저장하기, 불러오기, 데이터 입력, 쿼리 선택, 데이터베이스 선택 이 있으며, 이후에도 필요 시 추가 할 예정입니다.  
  
#### 헤더
  
```  
#pragma once
#include "Global.h"

class CMySQLDBManager
{
private:
	DECLARE_SINGLETONE(CMySQLDBManager)

	CMySQLDBManager();
	~CMySQLDBManager();
private:
	MYSQL* connection, conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	int DB_Num;
public:
	void Begin();
	void End();

	bool UseUserDB();
	bool UserTBL_QuerySelect();

	bool UserTBL_Load(UserInfo*& user);
	void UserTBL_Insert(UserInfo* user);

	void UpdateSQLResult() { sql_result = mysql_store_result(connection); }
	MYSQL_ROW UpdateSQLROW() 
	{
		sql_row = mysql_fetch_row(sql_result);
		return sql_row;
	}

	void SetConnection(MYSQL* _con) { connection = _con; }
	void SetConn(MYSQL _con) { conn = _con; }
	void SetSQLResult(MYSQL_RES* _result) { sql_result = _result; }
	void SetSQLROW(MYSQL_ROW _row) { sql_row = _row; }

	MYSQL* GetConnection() { return connection; }
	MYSQL GetConn() { return conn; }
	MYSQL_RES* GetSQLResult() { return sql_result; }
	MYSQL_ROW GetSQLRow() { return sql_row; }
	char* GetSQLRow(int i) { return sql_row[i]; }
};
```  
  
#### 구현부
  
```  
#include "MySQLDataBaseManager.h"
IMPLEMENT_SINGLETON(CMySQLDBManager)

CMySQLDBManager::CMySQLDBManager()
{
	connection = nullptr;
	ZeroMemory(&conn, sizeof(conn));
	sql_result = nullptr;
	sql_row = nullptr;
	DB_Num = 0;
}

CMySQLDBManager::~CMySQLDBManager()
{

}

void CMySQLDBManager::Begin()
{
	printf("MySQL Ver : %s\n", mysql_get_client_info());

	// 초기화
	mysql_init(&conn);

	// DB 연결
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&conn));
		exit(0);
	}
	else
	{
		printf("성공\n");
	}
}

bool CMySQLDBManager::UseUserDB()
{
	const char* query = "use userdb;";	// From 다음 DB에 존재하는 테이블 명으로 수정하세요
	int query_stat = mysql_query(connection, query);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return false;
	}

	mysql_set_character_set(connection, "euckr");
	return true;
}

bool CMySQLDBManager::UserTBL_QuerySelect()
{ 
	int query_stat = mysql_query(connection, "select * from usertbl;");
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return false;
	}

	UpdateSQLResult();
	return true;
}

bool CMySQLDBManager::UserTBL_Load(UserInfo*& user)
{
	if (UpdateSQLROW() != NULL)
	{
		user = new UserInfo(sql_row[ID], sql_row[PW], sql_row[NAME], ++DB_Num);
		return false;
	}
	return true;
}

void CMySQLDBManager::UserTBL_Insert(UserInfo* user)
{
	char query[MSGSIZE];

	sprintf_s(query, "insert into usertbl values('%s', '%s', '%s');", user->id, user->pw, user->name);
	
	int query_stat = mysql_query(connection, query);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return;
	}
}

void CMySQLDBManager::End()
{
	mysql_free_result(sql_result);
	mysql_close(connection);
}
```  
  
#### 3. 크리티컬 섹션
--------------------------------------------------------------  
  
IOCP 가 기반이라 하더라도 다른 클라이언트가 사용 중인 데이터는 중첩으로 사용하게 하면 분명히 문제가 되기 때문에 크리티컬 섹션을 이용하여 이를 방지하였습니다.  
다만, 매번 Enter, Leave 함수를 사용하는 것은 기능이 많아질수록 관리하기 힘들어지고 혹시라도 Leave 기능을 잊고 사용하지 않으면 치명적인 문제를 발생하기 때문에 단 한 번의 호출로 Enter 와 Leave 가 자동으로 되게끔 구현하였습니다.  
  
클래스 생성자, 소멸자의 원리를 이용하여 간단하게 구현하였고 생성자가 호출될 때 Enter, 소멸자가 호출될 때 Leave 하도록 설계하였습니다.  
템플릿을 사용해 어떤 클래스에서도 사용할 수 있도록 구현했습니다.  
  
#### 헤더
  
```  
#pragma once
#include "Global.h"

class CriticalSection_EX
{
private:
	CRITICAL_SECTION cs;
public:
	CriticalSection_EX()
	{
		InitializeCriticalSection(&cs);
	}

	~CriticalSection_EX()
	{
		DeleteCriticalSection(&cs);
	}

	void Enter()
	{
		EnterCriticalSection(&cs);
	}

	void Leave()
	{
		LeaveCriticalSection(&cs);
	}
};

template <class T>
class CMultiThreadSyns
{
private:
	static CriticalSection_EX cs;
public:
	class CLock
	{
	public:
		CLock()
		{
			cs.Enter();
		}

		~CLock()
		{
			cs.Leave();
		}
	};
};

template <class T>
CriticalSection_EX CMultiThreadSyns<T>::cs;
```  
  
#### 4. Listen 소켓
--------------------------------------------------------------  
  
Listen 소켓 클래스는 TCP 통신 준비와 Accept 기능을 담당하고 있습니다.  
  
#### 헤더
  
```  
#pragma once
#include "Error.h"

class CListenSocket
{
private:
	SOCKET sock;
public:
	CListenSocket();
	~CListenSocket();

	void TCP_Setting(const char* _id, const short _port);
	void Listen();
	void Listen(const int _count);
	SOCKET Accept();

	SOCKET GetListenSocket() { return sock; }

	void Release();
};
```  
  
#### 구현부
  
```  
#include "ListenSocket.h"

CListenSocket::CListenSocket()
{
	sock = NULL;
}

CListenSocket::~CListenSocket()
{
	Release();
}

void CListenSocket::TCP_Setting(const char* _id, const short _port)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) CError::GetInstance()->err_quit("socket");

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;

	if (_id == nullptr)
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr(_id);
	}

	addr.sin_port = htons(_port);

	int retval = bind(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR) CError::GetInstance()->err_quit("bind");
}

void CListenSocket::Listen()
{
	int retval = listen(sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) CError::GetInstance()->err_quit("listen");
}

void CListenSocket::Listen(const int _count)
{
	int retval = listen(sock, _count);
	if (retval == SOCKET_ERROR) CError::GetInstance()->err_quit("listen");
}

SOCKET CListenSocket::Accept()
{
	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);

	SOCKET client_sock = accept(sock, (SOCKADDR*)&addr, &addrlen);
	if (client_sock == INVALID_SOCKET) CError::GetInstance()->err_display("Accept");

	return client_sock;
}

void CListenSocket::Release()
{
	closesocket(sock);
}
```  
  
#### 5. TCP 소켓
--------------------------------------------------------------  
  
TCP 소켓 클래스는 순수하게 송신과 수신 기능만을 담당하고 통신에 필요한 멤버 변수를 가지고 있습니다.  
멤버 변수로 가지고 있는 WSAOverLapped_EX 구조체는 전달 받은 클라이언트의 정보를 식별하기 위해서 재정의 한 것 입니다.  
이후 Packing 클래스와 Client 클래스는 이 클래스를 상속 받아 사용하게 됩니다.  
  
#### WSAOverLapped_EX
  
```  
struct WSAOverLapped_EX
{
	WSAOVERLAPPED overlapped;
	LPVOID ptr;
	IO_TYPE type;
};
```  
  
#### 헤더
  
```  
#pragma once
#include "Error.h"
#include "CriticalSection.h"

class CTCPSocket : public CMultiThreadSyns<CTCPSocket>
{
protected:
	WSAOverLapped_EX recv_overlapped;
	WSAOverLapped_EX send_overlapped;

	SOCKET sock;
	SOCKADDR_IN addr;

	WSABUF wsa_recv_buf;
	WSABUF wsa_send_buf;

public:
	CTCPSocket();
	CTCPSocket(SOCKET _sock);
	~CTCPSocket();

	bool WSA_Recv(char* buf, int len);
	bool WSA_Send(char* buf, int len);

	bool WSA_Recv(char* buf, int len, const DWORD _count);
	bool WSA_Send(char* buf, int len, const DWORD _count);

	SOCKET GetSocket() { return sock; }
	SOCKADDR_IN GetAddr() { return addr; }

	void Release();
};
```  
  
#### 구현부
  
```  
#include "TCPSocket.h"

CTCPSocket::CTCPSocket()
{
	sock = NULL;
	ZeroMemory(&addr, sizeof(addr));

	recv_overlapped.ptr = this;
	recv_overlapped.type = IO_TYPE::IO_RECV;

	send_overlapped.ptr = this;
	send_overlapped.type = IO_TYPE::IO_SEND;
}

CTCPSocket::CTCPSocket(SOCKET _sock)
{
	sock = _sock;

	int addrlen = sizeof(addr);
	getpeername(sock, (SOCKADDR*)&addr, &addrlen);

	recv_overlapped.ptr = this;
	recv_overlapped.type = IO_TYPE::IO_RECV;

	send_overlapped.ptr = this;
	send_overlapped.type = IO_TYPE::IO_SEND;
}

CTCPSocket::~CTCPSocket()
{
	Release();
}

bool CTCPSocket::WSA_Recv(char* buf,  int len)
{
	DWORD recvbyte;
	DWORD flag = 0;
	ZeroMemory(&recv_overlapped.overlapped, sizeof(recv_overlapped.overlapped));

	wsa_recv_buf.buf = buf;
	wsa_recv_buf.len = len;

	int retval = WSARecv(sock, &wsa_recv_buf, 1, &recvbyte, &flag, &recv_overlapped.overlapped, nullptr);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("%d\n", WSAGetLastError());
			CError::GetInstance()->err_display("WSARecv");
			return false;
		}
	}
	return true;
}

bool CTCPSocket::WSA_Send(char* buf, int len)
{
	DWORD sendbyte;
	ZeroMemory(&send_overlapped.overlapped, sizeof(send_overlapped.overlapped));

	wsa_send_buf.buf = buf;
	wsa_send_buf.len = len;

	int retval = WSASend(sock, &wsa_send_buf, 1, &sendbyte, 0, &send_overlapped.overlapped, nullptr);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			CError::GetInstance()->err_display("WSASend");
			return false;
		}
	}
	return true;
}

bool CTCPSocket::WSA_Recv(char* buf, int len, const  
	DWORD _count)
{
	DWORD recvbyte, flag;
	ZeroMemory(&recv_overlapped.overlapped, sizeof(recv_overlapped.overlapped));

	wsa_recv_buf.buf = buf;
	wsa_recv_buf.len = len;

	int retval = WSARecv(sock, &wsa_recv_buf, _count, &recvbyte, &flag, &recv_overlapped.overlapped, nullptr);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			CError::GetInstance()->err_display("WSARecv");
			return false;
		}
	}
	return true;
}

bool CTCPSocket::WSA_Send(char* buf, int len, const DWORD _count)
{
	DWORD sendbyte;
	ZeroMemory(&send_overlapped.overlapped, sizeof(send_overlapped.overlapped));

	wsa_send_buf.buf = buf;
	wsa_send_buf.len = len;

	int retval = WSASend(sock, &wsa_send_buf, _count, &sendbyte, 0, &send_overlapped.overlapped, nullptr);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			CError::GetInstance()->err_display("WSASend");
			return false;
		}
	}
	return true;
}

void CTCPSocket::Release() 
{
	closesocket(sock);
}
```  
  
#### 6. 패킹
--------------------------------------------------------------  
  
이 클래스는 앞서 만든 TCP 소켓 클래스를 상속 받고 있습니다.
기본적으로 데이터를 자주 주고 받는 것 보단 한 번에 많은 양이라도 최소한으로 주고 받는 것이 좋기 때문에 보내고자 하는 데이터를 하나의 Pack 으로 만들어 보내게 됩니다.  
이 클래스에서는 Pack 을 만들어주고, 받은 Pack 을 다시 UnPack 해주는 기능을 담당하고 있습니다.  
또한 데이터를 잔여 데이터가 남지 않도록 온전히 송신, 수신을 할 수 있는 기능 역시 이 클래스가 담당하고 있습니다.  

또한 송신을 할 때 2개 이상의 송신 Pack 이 대기중인 경우, 자칫 데이터가 꼬일 수 있기 때문에 Queue 를 사용하여 데이터 꼬임을 방지하고 있습니다.  
  
#### 헤더
  
```  
#pragma once
#include "TCPSocket.h"

struct QueueSendData
{
	int sendbytes;
	int comp_sendbytes;

	char sendbuf[BUFSIZE];
};

class CPacking : public CTCPSocket
{
protected:
	int recvbytes;
	int comp_recvbytes;

	char recvbuf[BUFSIZE];

	bool recv_sizeflag;

	queue<QueueSendData*>* sendbuf;
public:
	CPacking();
	CPacking(SOCKET _sock);
	~CPacking();

	//이부분 수정필요
	unsigned __int64 GetProtocol();
	const char* UnPackData();

	bool Recv();
	int CompleteRecv(int _completebyte);

	void PackingData(unsigned __int64 protocol, const char* buf, int _size);

	bool Send();
	int CompleteSend(int _completebyte);

	
};
```  
  
#### 구현부
  
```  
#include "Packing.h"
#include"Crypt.h"

CPacking::CPacking()
{
	recvbytes = 0;
	comp_recvbytes = 0;
	ZeroMemory(recvbuf, sizeof(recvbuf));

	recv_sizeflag = false;

	sendbuf = new queue<QueueSendData*>();
}

CPacking::CPacking(SOCKET _sock) : CTCPSocket(_sock)
{
	recvbytes = 0;
	comp_recvbytes = 0;
	ZeroMemory(recvbuf, sizeof(recvbuf));

	recv_sizeflag = false;

	sendbuf = new queue<QueueSendData*>();
}

CPacking::~CPacking()
{
	delete sendbuf;
}

void CPacking::PackingData(unsigned __int64 protocol, const char* buf, int _size)
{
	CLock lock;
	char encrypt[4096];
	ZeroMemory(encrypt, sizeof(encrypt));
	QueueSendData* data = new QueueSendData;
	ZeroMemory(data, sizeof(QueueSendData));
	char* ptr = data->sendbuf;
	int size = 0;

	ptr = ptr + sizeof(size);



	memcpy(ptr, &protocol, sizeof(unsigned __int64));
	ptr = ptr + sizeof(unsigned __int64);
	size = size + sizeof(unsigned __int64);

	if (buf != nullptr)
	{
		memcpy(ptr, buf, _size);
		size = size + _size;
	}

	CCrypt::Decrypt((BYTE*)buf, (BYTE*)encrypt, size);
	//데이터 첫번째 전체 패킷 길이를 제외한 packing이 다된 상태에서 암호화를 진행 해야 하므로.

	ptr = data->sendbuf;
	memcpy(ptr, &size, sizeof(int));

	size = size + sizeof(int);
	data->sendbytes = size;

	sendbuf->push(data);
}

bool CPacking::Send()
{
	CLock lock;
	if (sendbuf->size() > 1)
	{
		printf("sendbuf->size() = %d\n", sendbuf->size());
		return false;
	}

	return WSA_Send(sendbuf->front()->sendbuf + sendbuf->front()->comp_sendbytes, sendbuf->front()->sendbytes - sendbuf->front()->comp_sendbytes);
}

int CPacking::CompleteSend(int _completebyte)
{
	CLock lock;

	sendbuf->front()->comp_sendbytes += _completebyte;

	if (sendbuf->front()->comp_sendbytes == sendbuf->front()->sendbytes)
	{
		sendbuf->pop();

		if (sendbuf->empty())
		{
			return SOC_TRUE;
		}
		else
		{
			if (!Send()) return SOC_ERROR;
			return SOC_FALSE;
		}
	}

	if (!Send()) return SOC_ERROR;

	return SOC_FALSE;
}
//이부분 수정
unsigned __int64 CPacking::GetProtocol()
{
	//CLock lock;

	unsigned __int64 protocol;
	memcpy(&protocol, recvbuf, sizeof(unsigned __int64));
	return protocol;
}

const char* CPacking::UnPackData()
{
	//CLock lock;
	//이부분 수정
	const char* ptr = recvbuf + sizeof(unsigned __int64);
	return ptr;
}

bool CPacking::Recv()
{
	//CLock lock;

	int len;
	if (recv_sizeflag)
	{
		len = recvbytes - comp_recvbytes;
	}
	else
	{
		len = sizeof(int) - comp_recvbytes;
	}

	return WSA_Recv(recvbuf + comp_recvbytes, len);
}

int CPacking::CompleteRecv(int _completebyte)
{
	//CLock lock;
	char decrypt[4096];
	ZeroMemory(decrypt, sizeof(decrypt));
	if (!recv_sizeflag)
	{
		comp_recvbytes += _completebyte;

		if (comp_recvbytes == sizeof(int))
		{
			memcpy(&recvbytes, recvbuf, sizeof(int));
			comp_recvbytes = 0;
			recv_sizeflag = true;
		}

		if (!Recv()) return SOC_ERROR;

		return SOC_FALSE;
	}

	comp_recvbytes += _completebyte;

	if (comp_recvbytes == recvbytes)
	{
		//CCrypt::Decrypt((BYTE*)recvbuf, (BYTE*)decrypt, recvbytes);//데이터가 다 받아진 상태에서 복호화를 진행 해야 하므로.
		comp_recvbytes = 0;
		recvbytes = 0;
		recv_sizeflag = false;

		return SOC_TRUE;
	}
	else
	{
		if (!Recv()) return SOC_ERROR;

		return SOC_FALSE;
	}
}
```  
  
#### 7. Client
------------------------------------------------------------------  
  
클라이언트들의 정보를 관리하는 클래스입니다.  
보통 게임 내에 Player 정보나 Room 정보 등을 관리하고 State 패턴 기능 역시 이 클래스에서 관리하게 됩니다.
아래는 State 패턴과 Player, Room 정보를 관리하고 있는 클라이언트 클래스 입니다.
  
#### 헤더
  
```  
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
```  
  
#### 구현부
  
```  
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
```  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
### 클라이언트 라이브러리
-----------------------------------------  
  
### 플레이어 동기화
-----------------------------------------
  
### 서버-클라이언트 보간
-----------------------------------------
  
## 클라이언트
-----------------------------------------
### 각종 유저 편의 기능
  
-----------------------------------------
