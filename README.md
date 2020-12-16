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
  + 클라이언트 세부 업무 : 게임 UI, 옵션, 타이틀 화면과 같은 게임 시스템 기능 및 플레이어 서버 동기화와 보간 기능  
  
이 문서에서는 클라이언트 업무에 대한 자료를 작성하지 않았습니다.  
  
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
   
# 서버
-----------------------------------------
## 서버 라이브러리
-----------------------------------------
  
서버 라이브러리는 IOCP 를 기반으로 설계되었으며 DB 는 MYSQL 과 연동하고 있습니다.  
  
### 1. Protocol
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
  
### 2. MYSQL
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
  
### 3. CriticalSection_EX
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
  
### 4. Listen Socket
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
  
### 5. TCP Socket
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
  
### 6. Packing
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
  
### 7. Client
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
  
### 8. State
-------------------------------------------------------------------------
  
기능을 세분화하여 관리를 더욱 쉽게 하기 위해 사용한 상태 패턴 입니다.
상태 패턴은 자동차의 기어와 같이 사용자가 사용 할 기능만 사용 할 수 있게 해줍니다.
  
#### 헤더  
  
```  
#pragma once
#include "Global.h"
class CClientSection;

class CState
{
protected:

public:
	CState()
	{

	}

	~CState()
	{

	}
	virtual unsigned __int64 Get_Sub_State() = 0;
	virtual void RecvProcess(CClientSection* ptr) = 0;
	virtual void SendProcess(CClientSection* ptr) = 0;
	// virtual void Disconnected(CClientSection* ptr) = 0;
};
```  
  
### 9. Server Manager
-----------------------------------------------------------------
  
서버에 중추적인 기능을 담당하고 있는 클래스 입니다.  
Accept, Recv, Send, Disconnected 등 서버의 메인 메커니즘이 돌아가는 Manager 입니다.
  
#### 헤더
  
```  
#pragma once
#include "CriticalSection.h"
#include "MySQLDataBaseManager.h"
#include "ListenSocket.h"
#include "IOCPManager.h"
#include "CProtocol.h"

#include "ClientManager.h"
#include "LoginManager.h"
#include "ChatManager.h"
#include "Lobby_Manager.h"
#include "Player_Manager.h"
#include "MonsterManager.h"

class CServerManager : public CIOCPManager
{
private:
	static CServerManager* pthis;
	vector<unsigned __int64> class_state;
	CServerManager();
	~CServerManager();

	CListenSocket* mListenSock;
public:
	static CServerManager* GetInstance();
	static void Destory();

	bool Begin();
	void Run();
	void End();

	int Recv(void* ptr, int len);
	int Send(void* ptr, int len);
	void Disconneted(void* ptr);
	void Accept(void* ptr);
};
```  
  
#### 구현부  
  
```  
#include "ServerManager.h"
#include "LoginState.h"
#include "ChatState.h"
#include "Lobby_State.h"
#include "Player_State.h"
#include "Monster_State.h"

CServerManager* CServerManager::pthis = nullptr;

CServerManager* CServerManager::GetInstance()
{
	/*Create 순서
	ERROR -> PROTOCOL -> ClientManager-> DB -> Login -> Lobby -> CHAT n PLAYER -> ServerManager
	*/
	CError::Create();
	CProtocol::Create();
	ClientManager::Create();
	CMySQLDBManager::Create();
	LoginManager::Create();
	Lobby_Manager::Create();
	ChatManager::Create();
	PlayerManager::Create();
	MonsterManager::Create();

	if (!pthis) pthis = new CServerManager();
	return pthis;
}

void CServerManager::Destory()
{
	/*Destroy 순서
	CHAT n PLAYER-> Lobby-> Login-> ClientManager-> DB -> PROTOCOL-> ERROR-> ServerManager
	*/
	ChatManager::Destroy();
	Lobby_Manager::Destroy();
	LoginManager::Destroy();
	ClientManager::Destroy();
	CMySQLDBManager::Destroy();
	CProtocol::Destroy();
	CError::Destroy();
	PlayerManager::Destroy();
	MonsterManager::Destroy();

	if (pthis) delete pthis;
}

CServerManager::CServerManager()
{
	mListenSock = nullptr;
}

CServerManager::~CServerManager()
{
	if (mListenSock != nullptr)
		delete mListenSock;
}

bool CServerManager::Begin()
{
	// WSA Setting
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return false;

	// IOCP Setting
	if (!BeginIOCP()) return false;

	// Listen Setting
	mListenSock = new CListenSocket();
	mListenSock->TCP_Setting(nullptr, PORT);
	mListenSock->Listen();

	// Manager Setting/
	CMySQLDBManager::GetInstance()->Begin();
	ClientManager::GetInstance()->Begin();
	LoginManager::GetInstance()->Begin();
	Lobby_Manager::GetInstance()->Begin();
	ChatManager::GetInstance()->Begin();
	PlayerManager::GetInstance()->Begin();
	MonsterManager::GetInstance()->Begin();

	/*
	state input
	INIT_STATE =  0x0100000000000000,
	LOGIN_STATE = 0x0200000000000000,
	CHAT_STATE =  0x0300000000000000,
	LOBBY_STATE = 0x0400000000000000,
	PLAYER_STATE = 0x0500000000000000,
	*/

	this->class_state.push_back((unsigned __int64)CLASS_STATE::INIT_STATE);
	this->class_state.push_back((unsigned __int64)CLASS_STATE::CHAT_STATE);
	this->class_state.push_back((unsigned __int64)CLASS_STATE::LOBBY_STATE);
	this->class_state.push_back((unsigned __int64)CLASS_STATE::LOGIN_STATE);
	this->class_state.push_back((unsigned __int64)CLASS_STATE::PLAYER_STATE);
	this->class_state.push_back((unsigned __int64)CLASS_STATE::MONSTER_STATE);

	return true;
}

void CServerManager::Run()
{
	while (1)
	{
		SOCKET sock = mListenSock->Accept();
		if (sock == INVALID_SOCKET)
		{
			CError::GetInstance()->err_display("Accept");
			break;
		}

		RegisterIOCP(sock, 0);

		WSAOverLapped_EX* wsa_accept = new WSAOverLapped_EX;
		memset(wsa_accept, 0, sizeof(WSAOverLapped_EX));

		wsa_accept->ptr = (void*)sock;
		wsa_accept->type = IO_ACCEPT;

		PostQueuedCompletionStatus(hcp, 0, sock, (LPOVERLAPPED)wsa_accept);
	}
}

void CServerManager::End()
{
	/*Destroy 순서
	CHAT n PLAYER-> Lobby-> Login-> ClientManager-> DB -> PROTOCOL-> ERROR-> ServerManager
	*/

	ChatManager::GetInstance()->End();
	Lobby_Manager::GetInstance()->End();
	LoginManager::GetInstance()->End();
	ClientManager::GetInstance()->End();
	CMySQLDBManager::GetInstance()->End();
	PlayerManager::GetInstance()->End();
	MonsterManager::GetInstance()->End();

	WSACleanup();
	Destory();
}

int CServerManager::Recv(void* ptr, int len)
{
	CClientSection* client = (CClientSection*)ptr;
	if (client == nullptr)
	{
		CError::GetInstance()->err_display("Recv");
		return true;
	}

	int result = client->CompleteRecv(len);

	switch (result)
	{
	case SOC_ERROR:
		return SOC_ERROR;
	case SOC_FALSE:
		return SOC_FALSE;
	case SOC_TRUE:
		break;
	}

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("[Time] %d/%d %d:%d:%d\n", tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	unsigned __int64 full_code = client->GetProtocol();

	for (int i = 0; i < class_state.size(); i++)
	{
		if (CProtocol::GetInstance()->ProtocolUnpacker(full_code, (unsigned __int64)class_state[i], NULL, NULL))
		{
			switch ((CLASS_STATE)class_state[i])
			{
			case CLASS_STATE::INIT_STATE:
				printf("INIT_STATE\n");
				break;
			case CLASS_STATE::LOGIN_STATE:
				printf("LOGIN_STATE\n");
				client->SetState(client->GetLoginState());
				break;
			case CLASS_STATE::CHAT_STATE:
				printf("CHAT_STATE\n");
				client->SetState(client->GetChatState());
				break;
			case CLASS_STATE::LOBBY_STATE:
				printf("LOBBY_STATE\n");
				client->SetState(client->GetLobbyState());
				break;
			case CLASS_STATE::PLAYER_STATE:
				printf("PLAYER_STATE\n");
				client->SetState(client->GetPlayerState());
				break;

			case CLASS_STATE::MONSTER_STATE:
				printf("MONSTER_STATE\n");
				client->SetState(client->GetMonaterState());
				break;
			}
			//초기화를 init_state로 했기 때문에 여기선 따로 설정하지 않는다. 시작과 동시에 CHAT_STATE 로 가게끔 설정해둠.		
		}
	}

	client->GetState()->RecvProcess(client);

	if (!client->Recv())
	{
		return SOC_ERROR;
	}

	return SOC_TRUE;
}

int CServerManager::Send(void* ptr, int len)
{
	CClientSection* client = (CClientSection*)ptr;
	if (client == nullptr)
	{
		CError::GetInstance()->err_display("Send");
		return true;
	}

	int result = client->CompleteSend(len);

	switch (result)
	{
	case SOC_ERROR:
		return SOC_ERROR;
	case SOC_FALSE:
		return SOC_FALSE;
	case SOC_TRUE:
		break;
	}

	client->GetState()->SendProcess(client);
	return SOC_TRUE;
}

void CServerManager::Accept(void* ptr)
{	
	printf("Accept\n");

	WSAOverLapped_EX* overlapped = (WSAOverLapped_EX*)ptr;
	SOCKET sock = (SOCKET)overlapped->ptr;
	// Add
	CClientSection* client = ClientManager::GetInstance()->AddClient(sock);

	delete overlapped;

	if (!client->Recv())
	{
		return;
	}
}

void CServerManager::Disconneted(void* ptr)
{
	printf("Disconneted\n");

	// Remove

	// 채팅 방 정보 제거
	// ChatManager::GetInstance()->Remove((CClientSection*)ptr);

	// 게임 방 정보 제거
	Lobby_Manager::GetInstance()->Remove((CClientSection*)ptr);

	// 로그인 중 비정상 종료 시 로그아웃 처리
	LoginManager::GetInstance()->Logout((CClientSection*)ptr);

	// 클라이언트 정보 제거
	ClientManager::GetInstance()->RemoveClient((CClientSection*)ptr);	
}
```  
  
### 10. IOCP Manager
----------------------------------------------------------------------
  
위 서버 라이브러리는 IOCP 를 사용하기 때문에 IOCP Manager 클래스를 설계 하였습니다.  
Server Manager 는 IOCP Manager 를 상속 받아 사용하게 됩니다.  
만약 IOCP 가 아닌 Multi Thread, Select 등 다른 방식을 사용하는 경우 그 클래스를 설계하고 ServerManager 가 상속 받아 사용하게끔 설계하면 됩니다.  
  
#### 헤더
  
```  
#pragma once
#include "ClientSection.h"

class CIOCPManager
{
protected:
	HANDLE hcp;
	vector<HANDLE>* hThreadList;
public:
	CIOCPManager();
	~CIOCPManager();

	bool BeginIOCP();
	void RegisterIOCP(SOCKET sock, int key);
	static DWORD WINAPI WorkerThread(LPVOID arg);
	void EndIOCP();

	virtual int Recv(void* ptr, int len) = 0;
	virtual int Send(void* ptr, int len) = 0;
	virtual void Disconneted(void* ptr) = 0;
	virtual void Accept(void* ptr) = 0;
};
```  
  
#### 구현부  
  
```  
#include "IOCPManager.h"

CIOCPManager::CIOCPManager()
{
	hcp = nullptr;
	hThreadList = nullptr;
}

CIOCPManager::~CIOCPManager()
{
	if (hThreadList != nullptr)
		delete hThreadList;
}

bool CIOCPManager::BeginIOCP()
{
	hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if (hcp == nullptr) return false;

	SYSTEM_INFO sys;
	GetSystemInfo(&sys);

	hThreadList = new vector<HANDLE>();

	for (int i = 0; i < (int)sys.dwNumberOfProcessors * 2; i++)
	{
		hThreadList->push_back(CreateThread(NULL, 0, WorkerThread, this, 0, NULL));
	}
}

DWORD WINAPI CIOCPManager::WorkerThread(LPVOID arg)
{
	int retval;
	CIOCPManager* ptr = (CIOCPManager*)arg;

	while (1)
	{
		DWORD cbTransferrd;
		SOCKET sock = NULL;
		WSAOverLapped_EX* overlapped;

		retval = GetQueuedCompletionStatus(ptr->hcp, &cbTransferrd, (PULONG_PTR)&sock, (LPOVERLAPPED*)&overlapped, INFINITE);

		if (sock == NULL && (retval == 0 || cbTransferrd == 0))
		{
			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(sock, &overlapped->overlapped, &temp1, false, &temp2);
				CError::GetInstance()->err_display("WSAGetOverlappedResult");
			}

			ptr->Disconneted(overlapped->ptr);
			continue;
		}

		switch (overlapped->type)
		{
		case IO_RECV:
		{
			int recv = ptr->Recv(overlapped->ptr, cbTransferrd);
			switch (recv)
			{
			case SOC_ERROR:
				ptr->Disconneted(overlapped->ptr);
			case SOC_FALSE:
				continue;
			}
		}
			break;

		case IO_SEND:
		{
			int send = ptr->Send(overlapped->ptr, cbTransferrd);
			switch (send)
			{
			case SOC_ERROR:
				ptr->Disconneted(overlapped->ptr);
			case SOC_FALSE:
				continue;
			}
		}
			break;

		case IO_ACCEPT:
			ptr->Accept(overlapped);
			break;
		}
	}

	return 0;
}

void CIOCPManager::RegisterIOCP(SOCKET sock, int key)
{
	CreateIoCompletionPort((HANDLE)sock, hcp, key, 0);
}

void CIOCPManager::EndIOCP()
{
	hcp = nullptr;
	hThreadList->clear();
}
```  
  
### 11. Client Manager
------------------------------------------------------------------  
  
클라이언트 정보를 관리하는 Manager 클래스 입니다.  
클라이언트를 추가, 삭제, 검색 하는 기능을 가지고 있고 클라이언트 리스트를 가지고 있습니다.  
  
#### 헤더
  
```  
#pragma once
#include "CriticalSection.h"
#include "ClientSection.h"

class ClientManager : public CMultiThreadSyns<ClientManager>
{
private:
	DECLARE_SINGLETONE(ClientManager)

	ClientManager();
	~ClientManager();
private:
	list<CClientSection*>* mClient_List;
	CClientSection* mClient_Data;
public:
	void Begin();
	void End();

	CClientSection* AddClient(SOCKET _sock);
	void RemoveClient(CClientSection* ptr);

	CClientSection* SearchClientInfo(const char* id);
	CClientSection* SearchClientData();

	//void SearchClientBegin();
	//void SearchClientEnd();
};
```  
  
#### 구현부  
  
```  
#include "ClientManager.h"
IMPLEMENT_SINGLETON(ClientManager)

ClientManager::ClientManager()
{
	mClient_List = nullptr;
	mClient_Data = nullptr;
}

ClientManager::~ClientManager()
{

}

void ClientManager::Begin()
{
	mClient_List = new list<CClientSection*>();
}

void ClientManager::End()
{
	if (mClient_List != nullptr) delete mClient_List;
	if (mClient_Data != nullptr) delete mClient_Data;
}

CClientSection* ClientManager::AddClient(SOCKET sock)
{
	CLock lock;

	CClientSection* client = new CClientSection(sock);
	mClient_List->push_back(client);

	printf("클라이언트 접속 : IP : %s , PORT : %d\n\n", inet_ntoa(client->GetAddr().sin_addr), ntohs(client->GetAddr().sin_port));
	return client;
}

void ClientManager::RemoveClient(CClientSection* ptr)
{
	CLock lock;

	printf("클라이언트 종료 : IP : %s , PORT : %d\n\n", inet_ntoa(ptr->GetAddr().sin_addr), ntohs(ptr->GetAddr().sin_port));
	if (ptr->IsLogin())
		ptr->Logout();
	mClient_List->remove(ptr);
	delete ptr;
}

CClientSection* ClientManager::SearchClientInfo(const char* id)
{
	CLock lock;

	mClient_Data = nullptr;
	
	for (list<CClientSection*>::iterator data = mClient_List->begin(); data != mClient_List->end(); data++)
	{
		mClient_Data = *data;

		if (!strcmp(mClient_Data->GetUser()->id, id))
		{
			break;
		}
	}

	return mClient_Data;
}

CClientSection* ClientManager::SearchClientData()
{
	return mClient_Data;
}
```  
  
### 게임 서버 구조  
--------------------------------------------------------------------------------------- 
  
![4](https://user-images.githubusercontent.com/63215359/102322067-d5045100-3fc1-11eb-8395-cfb80b29fe54.PNG)
  
  
본 프로젝트의 게임 서버에서 서버는 오로지 클라이언트에게 받은 데이터를 다른 클라이언트에게 다시 송신해주는 역할을 수행합니다.  
플레이어 캐릭터는 양측이 다 움직이기 때문에 모든 클라이언트가 다른 클라이언트에게 자신의 정보를 알려야 합니다.  
  
하지만 몬스터는 모든 클라이언트가 동기화가 되어야 하기 때문에 기준이 되는 클라이언트 한 명만이 다른 클라이언트에게 정보를 알리게 됩니다.  
이 때 몬스터는 서버에게 데이터를 받은 후에 렌더링이 되어야 하는데 데이터를 받기 전에 렌더링이 된다면 동기화에 문제가 생길 수 있기 때문입니다.  
다만, 피격에 관해서는 예외적으로 모든 클라이언트가 서버에게 피격 정보를 주어야 하며 몬스터 피격 처리는  
클라이언트가 아닌 서버에서 처리하며, 클라이언트들에게 피격 계산 후의 몬스터 HP 데이터를 수신하게 됩니다.  
  
Player, Monster, Login, Match 등의 Manager 클래스는 이 문서에 기재하지 않았고, Script 를 Commit 하여 따로 기재 하였습니다.  
  
  
## 클라이언트 라이브러리
-----------------------------------------  
  
Unity 3D 는 C# 을 사용하고 있기 때문에 C# TCP 클라이언트 라이브러리를 설계하게 되었습니다.  
  
### 1. TCP Socket
-------------------------------------------------------------------------------
  
클라이언트에서는 IOCP 를 사용하지 않고 TCP Socket 만 사용합니다.  
Connect, Send, Recv 기능을 담당하고 있으며, Pack, UnPack 을 위한 Buffer 구조체를 재정의 하였습니다.  
  
#### 소스
  
```  
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.Net;
using System.Net.Sockets;

namespace TCP
{
    public class SendBuffer
    {
        public byte[] buffer;
        public int offset;
        public int size;
    }

    public class RecvBuffer
    {
        public byte[] buffer;
        public int offset;
        public int size;

        public RecvBuffer()
        {
            buffer = null;
            offset = 0;
            size = 0;
        }
    }
    
    public class TCPSocket : MonoBehaviour
    {
        public TcpClient Sock;
        public NetworkStream Net;

        public bool Connecting(string ip, int port)
        {
            try
            {
                Sock = new TcpClient(ip, port);
                Net = Sock.GetStream();
            }
            catch (SocketException e)
            {
                print("Socket Error : " + e.Message.ToString());
                return false;
            }
            catch (Exception e)
            {
                print("ETC Error : " + e.Message.ToString());
                return false;
            }
            return true;
        }

        public void Send(SendBuffer ptr)
        {
            Net.Write(ptr.buffer, 0, ptr.size);
        }

        public int Recv(RecvBuffer ptr)
        {
            return Net.Read(ptr.buffer, ptr.offset, ptr.size);
        }

        public void Release()
        {
            Net.Close();
            Sock.Close();
        }
    }
}
```  
  
### 2. Packing
----------------------------------------------------------------------------  
  
수신하고자 하는 데이터를 Pack 해주고, 수신 받은 데이터를 UnPack 해주는 기능을 담당하는 클래스 입니다.  
또한 송신, 수신을 위한 스레드를 생성하고 Queue 를 통해 이를 관리하게 됩니다.  
  
#### 소스  
  
```  
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.Threading;
using System.Net.Sockets;
using UnityEngine.Playables;

namespace TCP
{
    public class Packing : TCPSocket
    {        
        public Queue<SendBuffer> m_SendBuffer;
        public Queue<RecvBuffer> m_RecvBuffer;

        Thread SendThread;
        Thread RecvThread;

        public AutoResetEvent m_SendEvent;

        static public object m_lock;
        public static List<string> ip = new List<string>();

        public void Begin()
        {
            //ip.Add("192.168.0.216");
            //ip.Add("127.0.0.1");

            // 로컬
            ip.Add("182.172.23.98");

            // AWS -----------------
            // 서울
            ip.Add("54.180.123.104");

            // 오하이오 ( 미국 )
            ip.Add("3.137.204.197");

            // 일본
            ip.Add("13.112.127.182");

            bool connected = false;

            foreach (var item in ip)
            {
                if (Connecting(item, 9050))
                {
                    connected = true;
                    break;
                }
            }

            if (!connected)
                Application.Quit();

            //if (!Connecting(ip, 9050))
            //{

            //    if (!Connecting(ip, 9050))
            //        Application.Quit();
            //}

            m_lock = new object();
            m_SendEvent = new AutoResetEvent(false);

            m_SendBuffer = new Queue<SendBuffer>();
             m_RecvBuffer= new Queue<RecvBuffer>();

            SendThread = new Thread(new ThreadStart(SendProcess));
            RecvThread = new Thread(new ThreadStart(RecvProcess));

            SendThread.Start();
            RecvThread.Start();
        }

        public void End()
        {
            m_SendBuffer.Clear();
            m_RecvBuffer.Clear();

            SendThread.Abort();
            RecvThread.Abort();

            Release();
        }

        void SendProcess()
        {
            while (true)
            {
                try
                {
                    if (m_SendBuffer.Count == 0)
                        m_SendEvent.WaitOne();

                    lock (m_lock)
                        Send(m_SendBuffer.Dequeue());
                }
                catch
                {
                    //print("Send Error");
                    continue;
                }
            }
        }

        void RecvProcess()
        {
            while (true)
            {
                try
                {
                    RecvBuffer data = new RecvBuffer();

                    data.buffer = new byte[4];
                    data.size = sizeof(int);
                    data.offset = 0;

                    int retval = Recv(data);
                    data.size = BitConverter.ToInt32(data.buffer, 0);
                    data.buffer = new byte[data.size];

                    while (data.size > 0)
                    {
                        retval = Recv(data);
                        if (retval == 0)
                            break;

                        data.size -= retval;
                        data.offset += retval;
                    }

                    data.size = data.offset;

                    lock (m_lock)
                        m_RecvBuffer.Enqueue(data);
                }
                catch
                {
                    //print("Recv Error");
                    continue;
                }
            }

        }

        public void PackingData(UInt64 Protocol)
        {
            lock (m_lock)
            {
                SendBuffer data = new SendBuffer();
                data.buffer = new byte[4096];
                data.size = 0;

                data.offset = sizeof(int);

                Buffer.BlockCopy(BitConverter.GetBytes(Protocol), 0, data.buffer, data.offset, sizeof(UInt64));
                data.size += sizeof(UInt64);

                Buffer.BlockCopy(BitConverter.GetBytes(data.size), 0, data.buffer, 0, sizeof(int));
                data.size += sizeof(int);

                m_SendBuffer.Enqueue(data);
                m_SendEvent.Set();
            }
        }

        public void PackingData(UInt64 Protocol, byte[] buffer)
        {
            lock (m_lock)
            {

                SendBuffer data = new SendBuffer();
                data.buffer = new byte[4096];
                data.size = 0;

                data.offset = sizeof(int);

                Buffer.BlockCopy(BitConverter.GetBytes(Protocol), 0, data.buffer, data.offset, sizeof(UInt64));
                data.offset += sizeof(UInt64);
                data.size += sizeof(UInt64);

                if (buffer.Length != 0)
                {
                    Buffer.BlockCopy(buffer, 0, data.buffer, data.offset, buffer.Length);
                    data.size += buffer.Length;
                }

                Buffer.BlockCopy(BitConverter.GetBytes(data.size), 0, data.buffer, 0, sizeof(int));
                data.size += sizeof(int);

                m_SendBuffer.Enqueue(data);
                m_SendEvent.Set();
            }
        }


        public UInt64 GetProtocol()
        {
            lock (m_lock)
            {
                try
                {
                    return BitConverter.ToUInt64(m_RecvBuffer.Peek().buffer, 0);
                }
                catch
                {
                    return 0;
                }
            }
        }

        public RecvBuffer UnPackingData()
        {
            lock (m_lock)
            {
                try
                {
                    return m_RecvBuffer.Dequeue();
                }
                catch
                {
                    return new RecvBuffer();
                }
            }
        }
    }
}
```  
  
### 3. Client  
----------------------------------------------------------------------------  
  
State 패턴을 사용하여 지정된 기능만 사용 할 수 있게 하는 기능을 담당하고 있습니다.  
또한 싱글톤으로 접근이 간단하고, 한 클라이언트에 한 개 이상의 Client 객체가 있는 경우를 방지하고 있습니다.  
  
#### 소스
  
```  
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;

namespace TCP
{
    public class TCPClient : Packing
    {
        public static TCPClient Instance;

        State m_State;

        [HideInInspector] public static IntroState m_Intro;
        [HideInInspector] public static LoginState m_Login;
        [HideInInspector] public static MatchState m_Match;
        [HideInInspector] public static Player_State m_Player;
        [HideInInspector] public static MonsterState m_Monster;

        // Start is called before the first frame update
        void Awake()
        {
            DontDestroyOnLoad(this);

            if (Instance != null)
            {
                GameObject obj = GameObject.Find("ServerManager");
                DestroyImmediate(obj);
            }
            else
            {
                Instance = this;
                gameObject.name = "ServerManager_Orizin";
                Begin();

                m_Intro = new IntroState();
                m_Login = new LoginState();
                m_Match = new MatchState();
                m_Player = new Player_State();
                m_Monster = new MonsterState();

                m_Intro.Intro_Message();
            }
        }

        // Update is called once per frame
        void Update()
        {
            if (m_RecvBuffer.Count != 0)
            {
                var Class_State = GetProtocol() & (UInt64)FULL_CODE.MAIN;

                switch((CLASS_STATE)Class_State)
                {
                    case CLASS_STATE.INIT_STATE:
                        SetState(m_Intro);
                        break;

                    case CLASS_STATE.LOGIN_STATE:
                        SetState(m_Login);
                        break;

                    case CLASS_STATE.LOBBY_STATE:
                        SetState(m_Match);
                        break;

                    case CLASS_STATE.PLAYER_STATE:
                        SetState(m_Player);
                        break;

                    case CLASS_STATE.MONSTER_STATE:
                        SetState(m_Monster);
                        break;
                }

                m_State.RecvProcess();
            }
        }

        void SetState(State _state)
        {
            m_State = _state;
        }

        private void OnApplicationQuit()
        {         
            End();
        }
    }
}
```  
  
### 4. State
-----------------------------------------------------------------  
  
상태 패턴을 사용하여 사용 할 기능만을 사용할 수 있습니다.
  
#### 소스  
  
```  
public abstract class State : TCPClient
{
    public abstract void RecvProcess();
    public abstract void SendProcess();
}
```  
  
### 4-1. Player State
----------------------------------------------------------------  
  
Player, Monster, Login, Match 등 State 패턴으로 관리하고 있는 기능 중 Player State 만 이 문서에 기재합니다.  
송신 시 사용 할 기능을 정의하고 수신 시 Protocol 를 분리하여 어떤 기능을 할 것인가를 결정하게 됩니다.  
TCPClient 를 상속받고 있기 때문에 접근이 간단하며 Manager 나 Control 클래스에서 송수신에 관련된 기능을 쉽게 사용할 수 있습니다.  
  
#### 소스
  
```  
using System;
using System.Text;
using TCP;
using UnityEngine;

public class Player_State : State
{   
    public void Player_BeginInfoUpdate(int hp)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.UPDATE | (UInt64)PROTOCOL.BEGININFO;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, hp, Static_Data.m_jab));
    }

    public void Player_TransformUpdate(Vector3 position, Quaternion rotation)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.UPDATE | (UInt64)PROTOCOL.TRANSFROM;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, position, rotation));
    }

    public void Player_HPUpdate(UInt64 PROTOCOL, int HP)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.UPDATE | PROTOCOL;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, HP));
    }

    public void Player_UpdateRotation(Quaternion rotation)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.UPDATE | (UInt64)PROTOCOL.ROTATION;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, rotation));
    }

    public void Player_Movement(Vector3 position, Quaternion rotation, float anim)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.MOVEMENT | (UInt64)PROTOCOL.MOVE;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, position, rotation, anim));
    }

    public void Player_FocusMovement(Vector3 position, Quaternion rotation, float forward, float right)
    {
        UInt64 Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.MOVEMENT | (UInt64)PROTOCOL.FOCUS;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, position, rotation, forward, right));
    }

    public void Player_Animation_Active(UInt64 _Protocol, string anim)
    {
        var Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.ANIMATION | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, anim));
    }

    public void Player_Animation_Active(UInt64 _Protocol, Vector3 position, Quaternion rotation, string anim)
    {
        var Protocol = (UInt64)CLASS_STATE.PLAYER_STATE | (UInt64)STATE.ANIMATION | _Protocol;
        TCPClient.Instance.PackingData(Protocol, PackingData(Static_Data.m_number, position, rotation, anim));
    }

    public Vector3 prev_position = Vector3.zero;
    public Quaternion prev_rotation = Quaternion.identity;
    public int prev_hp = 0;
    public string nickname;

    public override void RecvProcess()
    {
        UInt64 State = TCPClient.Instance.GetProtocol() & (UInt64)FULL_CODE.SUB;
        UInt64 Protocol = TCPClient.Instance.GetProtocol() & (UInt64)FULL_CODE.PROTOCOL;

        var buffer = TCPClient.Instance.UnPackingData();
        if (buffer.size == 0)
        {
            print("Queue Empty");
            return;
        }

        Debug.Log(string.Format("temp = {0:x}", Protocol));

        int num = 0;
        Vector3 position = Vector3.zero;
        Quaternion rotation = Quaternion.identity;

        string string_anim = string.Empty;
        float float_anim = 0f;
        int value = 0;

        try
        {
            var ohter = GameObject.Find("Other").GetComponent<OtherControl>();

            switch ((STATE)State)
            {
                case STATE.MOVEMENT:
                    switch ((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.MOVE:
                            UnPackingData(buffer, out num, out position, out rotation, out float_anim);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_Movement(position, rotation, float_anim);
                            break;

                        case PROTOCOL.FOCUS:
                            print("Focus Move");
                            float forward, right;
                            UnPackingData(buffer, out num, out position, out rotation, out forward, out right);

                            ohter.Other_FocusMovement(position, rotation, forward, right);
                            break;
                    }
                    break;

                case STATE.UPDATE:
                    switch ((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.BEGININFO:
                            UnPackingData(buffer, out num, out Static_Data.m_OhterName, out Static_Data.m_OhterJab);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                            {
                                ohter.Other_InfoSetting(Static_Data.m_OhterName);
                            }
                            break;

                        case PROTOCOL.HP:
                            UnPackingData(buffer, out num, out value);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_HP(value);
                            break;

                        case PROTOCOL.TRANSFROM:
                            UnPackingData(buffer, out num, out position, out rotation);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_UpdateTransform(position, rotation);
                            break;
                    }
                    break;

                case STATE.ANIMATION:
                    switch ((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.RESURRECTION:
                        case PROTOCOL.GUN:
                        case PROTOCOL.SERUM:
                            UnPackingData(buffer, out num, out position, out rotation, out string_anim);
                            print(string_anim);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_Animation_Active(position, rotation, string_anim);
                            break;

                        case PROTOCOL.HIT:
                            UnPackingData(buffer, out num, out string_anim);
                            print(string_anim);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_Hit(string_anim);
                            break;

                        case PROTOCOL.CHARGEDOWN:
                            UnPackingData(buffer, out num, out string_anim);
                            print(string_anim);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_ChargeDown();
                            break;

                        case PROTOCOL.CHARGEUP:
                            UnPackingData(buffer, out num, out string_anim);
                            print(string_anim);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_ChargeUp();
                            break;

                        case PROTOCOL.EVADE:
                        case PROTOCOL.ATTACK:
                        case PROTOCOL.DIE:
                            UnPackingData(buffer, out num, out string_anim);
                            print(string_anim);

                            if (ohter == null)
                                print("Not Search Ohter Object");
                            else
                                ohter.Other_Animation_Active(string_anim);
                            break;
                    }
                    break;
            }
        }
        catch
        {
            // 아직 플레이어가 로딩 중인 경우
            print("Not Loading Complete");

            switch ((STATE)State)
            {
                case STATE.MOVEMENT:
                    switch ((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.MOVE:
                            UnPackingData(buffer, out num, out prev_position, out prev_rotation, out float_anim);
                            break;
                    }
                    break;

                case STATE.UPDATE:
                    switch ((PROTOCOL)Protocol)
                    {
                        case PROTOCOL.BEGININFO:
                            UnPackingData(buffer, out num, out Static_Data.m_OhterName, out Static_Data.m_OhterJab);
                            break;

                        case PROTOCOL.TRANSFROM:
                            UnPackingData(buffer, out num, out prev_position, out prev_rotation);
                            break;

                        case PROTOCOL.HP:
                            UnPackingData(buffer, out num, out prev_hp);
                            break;
                    }
                    break;
            }
        }
    }
}
```  
    
## 플레이어 동기화
-----------------------------------------
  
각 클라이언트 별로 움직이는 플레이어 캐릭터에 대한 동기화는 Other 라는 오로지 다른 클라이언트에게 받은 정보로 움직이는 오브젝트를 만들어 처리하였습니다.  
Ohter 마다 고유 번호를 가지고 있으며, 서버 또한 각 클라이언트의 고유 번호를 가지고 있어 각각의 Ohter 오브젝트가 누구의 캐릭터인 지 식별이 가능합니다.  
애니메이션 또한 플레이어 캐릭터의 Animation Controller 내에 키값을 서버에게 받아 그래도 출력해 줌으로써 동기화에 성공하였습니다.  
  
이동 시 Lerp 를 사용한 이유는 서버와 클라이언트 간 보간을 위해서 사용하였습니다.  
  
#### 소스
  
```  
using System;
using System.Collections;
using System.Collections.Generic;
using TCP;
using UnityEngine;
using UnityEngine.Experimental.PlayerLoop;

public class OtherControl : MonoBehaviour
{    
    public Animator Anim = null;
    CharacterController CC = null;
    PlayerAnimControl player_anim = null;
    Health m_Health;

    public string nickname;
    Vector2 AnimValue_2D;
    bool FocusState;

    public bool In;

    public Vector3 client_position;
    Quaternion client_rotation;

    public GameObject Black;
    public GameObject White;

    public GameObject White_gAttack, White_BPAttack;


    public Wallchecktrigger player_wc;


    void OtherType_Setting()
    {
        if (Black == null)
            Black = transform.Find("NewModelFix").gameObject;
        if (White == null)
            White = transform.Find("MODEL2").gameObject;

        if (Static_Data.m_OhterJab == "Player1")
        {
            player_wc.pac = Black.GetComponent<PlayerAnimControl>();
            Black.SetActive(true);
            White.SetActive(false);
        }
        else
        {
            player_wc.pac = White.GetComponent<PlayerAnimControl>();
            Black.SetActive(false);
            White.SetActive(true);

            gAttack = White_gAttack;
            bpAttack = White_BPAttack;
        }

        Anim = GetComponentInChildren<Animator>();
        player_anim = GetComponentInChildren<PlayerAnimControl>();

        Anim.SetBool("BasicState", true);
        StartCoroutine(AnimationUpdate());
    }

    IEnumerator Start()
    {
        yield return new WaitForSeconds(0.01f);
        if (Static_Data.m_OhterJab != string.Empty)
            OtherType_Setting();

        client_position = transform.position;
        client_rotation = transform.rotation;

        CC = GetComponent<CharacterController>();
        m_Health = GetComponent<Health>();

        AnimValue_2D = Vector2.zero;
        FocusState = false;

        if (Static_Data.m_OhterName != string.Empty)
            nickname = Static_Data.m_OhterName;

        if (TCPClient.m_Player != null && TCPClient.m_Player.prev_hp != 0)
            Other_HP(TCPClient.m_Player.prev_hp);
    }

    bool PushObjectBackInFrustum()
    {
        if (Physics.Linecast(transform.position, Camera.main.transform.position, 1 << 14))
        {
            return true;
        }

        if (Camera.main != null)
        {
            Vector3 pos = Camera.main.WorldToViewportPoint(transform.position);

            if ((pos.x < 0f || pos.x > 1f || pos.y < 0f || pos.y > 1f || pos.z < 0f) || Anim == null)
                return true;
            else return false;
        }
        return false;
    }

    // Update is called once per frame
    void Update()
    {
        Transform_Lerp();

        if (PushObjectBackInFrustum())
        {
            In = false;
        }
        else
        {
            In = true;
        }
    }

    IEnumerator AnimationUpdate()
    {
        StartCoroutine(Attack_Effect());

        while (true)
        {
            if (Anim.GetBool("BasicState"))
            {
                Anim.SetFloat("Forward", AnimValue_2D.x, 0.1f, Time.deltaTime);
            }
            else if (Anim.GetBool("FocusState"))
            {
                Anim.SetFloat("Forward", AnimValue_2D.x, 0.1f, Time.deltaTime);
                Anim.SetFloat("Right", AnimValue_2D.y, 0.1f, Time.deltaTime);

            }
            yield return null;
        }
    }

    public void Other_InfoSetting(string name)
    {
        nickname = name;
        OtherType_Setting();
        print(name);
    }

    void Ohter_AnimationMovement()
    {
        if (Anim.GetBool("BasicState"))
        {            
            if (/*Anim.GetBool("Attacking") || Anim.GetBool("BPAttacking") || Anim.GetBool("ChargeAttacking") || */Anim.GetBool("UseEvade"))
            {
                CC.Move(transform.forward * player_anim.TranslateSpeed * Time.deltaTime);
            }
        }
	
        client_position = transform.position;
        client_rotation = transform.rotation;
    }

    void Transform_Lerp()
    {
        var lerppos = Vector3.Lerp(transform.position, client_position, 0.1f);
        transform.position = lerppos;

        var lerprot = Quaternion.Lerp(transform.rotation, client_rotation, 0.2f);
        transform.rotation = lerprot;

        if (Vector3.Distance(transform.position, client_position) < 0.2f && animation_active)
        {
            transform.position = client_position;
            transform.rotation = client_rotation;

            Anim.SetTrigger(animation_name);
            animation_active = false;
        }

        if (AnimValue_2D != Vector2.zero)
        {
            if (Vector3.Distance(transform.position, client_position) < 0.2f)
            {
                if (AnimValue_2D.sqrMagnitude < 0.1f)
                    AnimValue_2D = Vector2.zero;
                else
                {
                    AnimValue_2D.x = Mathf.Lerp(AnimValue_2D.x, 0f, 0.2f);
                    AnimValue_2D.y = Mathf.Lerp(AnimValue_2D.y, 0f, 0.2f);
                }
            }
        }
    }

    public void Other_Movement(Vector3 pos, Quaternion rot, float anim)
    {
        client_position = pos;
        client_rotation = rot;              

        Anim.SetBool("FocusState", false);
        Anim.SetBool("BasicState", true); // 기본상태값을 true화시킨다.
        AnimValue_2D.x = anim;       
    }

    public void Other_FocusMovement(Vector3 pos, Quaternion rot, float forward, float right)
    {
        client_position = pos;
        client_rotation = rot;

        Anim.SetBool("FocusState", true);
        Anim.SetBool("BasicState", false); // 기본상태값을 true화시킨다.

        AnimValue_2D.x = forward;
        AnimValue_2D.y = right;
    }

    public GameObject gAttack;
    public GameObject bpAttack;

    IEnumerator Attack_Effect()
    {
        var playerAnimControl = GetComponentInChildren<PlayerAnimControl>();

        while (true)
        {
            // 플레이어가 공격중이라면
            if (Anim.GetBool("Attacking") || Anim.GetBool("BPAttacking") || Anim.GetBool("ChargeAttacking"))
            {
                if (Anim.GetBool("BPAttacking") && playerAnimControl.effect_run) // 블러드 포이트 공격중이라면
                {
                    // 블러드 포인트 공격 이펙트 켜기
                    gAttack.SetActive(false);
                    bpAttack.SetActive(true);
                }
                else if (Anim.GetBool("Attacking") && playerAnimControl.effect_run) // 일반 공격중이라면
                {
                    // 일판 공격 이펙트 켜기
                    bpAttack.SetActive(false);
                    gAttack.SetActive(true);
                }
                else if (Anim.GetBool("ChargeAttacking") && playerAnimControl.effect_run)
                {
                    // 차징 공격 이펙트 켜기
                    // 일단 기본 공격
                    bpAttack.SetActive(true);
                    gAttack.SetActive(false);
                }
                else
                {
                    gAttack.SetActive(false);
                    bpAttack.SetActive(false);
                }
            }
            else // 플레이어가공격중이 아니라면
            {
                gAttack.SetActive(false);
                bpAttack.SetActive(false);
            }
            yield return null;
        }
    }

    public void Other_UpdateTransform(Vector3 pos, Quaternion rot)
    {
        client_position = pos;
        client_rotation = rot;
    }

    public void Other_PrevUpdateTransform(Vector3 pos, Quaternion rot)
    {
        client_position = pos;
        client_rotation = rot;

        transform.position = pos;
        transform.rotation = rot;
    }

    public void Other_HP(int value)
    {
        m_Health.curHP = value;
    }

    public void Other_Hit(string name)
    {
        if (!Anim.GetBool("OnHit"))
        {
            if (name == "BigHit2")
            {
                Anim.SetFloat("Forward", 0f);
                Anim.SetFloat("Right", 0f);

                Anim.SetTrigger(name);
            }
            else
                Anim.SetBool(name, true);
        }
    }

    public void Other_ChargeDown()
    {
        Anim.SetFloat("Forward", 0f);
        Anim.SetFloat("Right", 0f);

        if (Anim.GetBool("Attacking"))
        {
            player_anim.Combo_Check_End();
            Anim.SetBool("NextKey", true);
        }
        else if (Anim.GetBool("BPAttacking"))
        {
            player_anim.BPCombo_Attack_End();
            Anim.SetBool("NextKey", true);
        }
        else if (Anim.GetBool("GunAttacking"))
        {
            player_anim.Gun_Attack_End();
            Anim.SetBool("NextKey", true);
        }

        Anim.SetBool("ChargeButtonDown", true);
    }

    public void Other_ChargeUp()
    {
        Anim.SetBool("ChargeButtonDown", false);
    }

    public void Other_Animation_Active(string name)
    {
        Anim.SetFloat("Forward", 0f);
        Anim.SetFloat("Right", 0f);
  
        Anim.SetTrigger(name);
    }

    bool animation_active = false;
    string animation_name = string.Empty;

    public void Other_Animation_Active(Vector3 pos, Quaternion rot, string name)
    {
        client_position = pos;
        client_rotation = rot;

        if (name != "Alive")
        {
            animation_name = name;
            animation_active = true;
        }
        else
        {
            transform.position = pos;
            transform.rotation = rot;

            Anim.SetFloat("Forward", 0f);
            Anim.SetFloat("Right", 0f);

            Anim.SetTrigger(name);
        }
    }
}
```  
  
## 서버-클라이언트 보간
-----------------------------------------
  
사실 실시간 멀티 게임 서버에서 매 프레임마다 서버와 클라이언트가 패킷을 주고 받을 수 있다면 여러 기법을 필요가 없었겠지만 아쉽게도 그것은 불가능 하였습니다.  
그렇기에 선대에 많은 프로그래머 분들이 고민하고 고민하여 다양한 기법들을 만들어 왔고, 저 역시 그 기법들 중 하나를 채택하여 사용하게 되었습니다.  
  
제가 채택한 기법은 데드 레커닝 기법으로 신호가 없을 때 어떻게 처리할 것인가? 에 대한 기법이었습니다.  
만약 5 프레임마다 보내게 된다면 0~4 프레임 동안 신호를 받지 못하게 되고, 캐릭터의 움직임이 끊기게 되는 현상을 해결하고자  
저는 0 -> 5 프레임까지의 최종 위치값을 방향 벡터를 통해 예상하고 예상 위치값으로 Lerp 를 통해 이동시켜 성공적으로 보간 문제를 해결했습니다.  
  
#### 소스
  
```  
	// 매 5 프레임 마다 예상 위치값 계산 후 전달
	if (FrameCheck > 5)
        {
            FrameCheck = -1;
            var pos = transform.position + (transform.forward * moveSpeed * Time.deltaTime * 5);

            try
            {
                if (Other.activeInHierarchy)
                    TCPClient.m_Player.Player_Movement(pos, transform.rotation, InputMagnitude);
            }
            catch
            {
                moving = false;
            }
        }
```  
  
추가로 소스에는 없지만 이동이 완료되었거나, 도중 이동이 취소되는 경우 ( ex. 공격, 피격, 회피 등 ) 에는 예상 위치값이 아닌 현재 위치값을 보내줍니다.  
공격이나 회피와 같이 캐릭터 좌표가 이동되는 경우에도 데드 레커닝 기법이 적용되어 있습니다.  
  
# 그 외  
-------------------------------------------  
## 맡은 역할
-------------------------------------------
  
서버와 클라이언트를 모두 담당했던 포지션으로  
서버 라이브러리 설계와 클라이언트 라이브러리 설계, 서버와 클라이언트 간 보간 작업, 플레이어 동기화 등을 맡았고,  
클라이언트 부분에서는 게임 유저 편의 시스템, 타이틀 메뉴 시스템, 캐릭터 선택 시스템 등 게임 시스템 부분을 담당 하였습니다.  
  
## 프로젝트를 진행하며 어려웠던 점
--------------------------------------------  
  
작업을 함에 있어서는 크게 어려웠던 점이 없었으나, 적지 않은 사람들과 각기 다른 분야에서 협업을 하며 게임을 제작함에 있어서 다소 어려움이 많았던 것 같았습니다.  
아직 다른 파트에서 작업을 완료하지 못하여 일정에 차질이 생기거나, 학업에도 신경을 써야 했던 점이 조금 어려웠습니다.  
개인적으로 가장 힘들었던 시기는 예상치 못한 일로 인해 제게 다른 파트의 일이 모두 맡겨졌었고, 엄청난 부담감으로 인해 힘들었던 것 같았습니다.  
  
## 개선되거나 학습한 점  
--------------------------------------------  
  
프로젝트를 하면서 혼자서는 할 수 없는 일이 같이 하면 충분히 할 수 있게 된다는 점을 일단 많이 배웠던 것 같습니다.  
막히는 부분이 있다면 서로 의견을 나누며 끝내 해결했던 과정이 제일 값졌던 것 같고, 타인과의 소통에 있어서도 많은 부분을 배웠습니다.  
특히 타협이라는 부분에서 많이 개선할 수 있었던 것 같습니다.  
  
무엇보다 제일 힘든 시기를 끝내 이겨낼 수 있었던 건 분명 옆에서 같이 고민해주던 팀원들이 있었던 덕분이라는 점을 알 수 있었습니다.  
  
