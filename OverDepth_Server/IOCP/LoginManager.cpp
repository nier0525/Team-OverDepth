#include "LoginManager.h"
#include "ChatManager.h"
IMPLEMENT_SINGLETON(LoginManager)

LoginManager::LoginManager()
{
	mUser_List = nullptr;
	mNowLoginUser_List = nullptr;
	protocol = CProtocol::GetInstance();
}
LoginManager::~LoginManager()
{
}

bool LoginManager::UserDataLoad()
{
	// MySQL 데이터 불러오기
	if (!CMySQLDBManager::GetInstance()->UseUserDB()) return false;
	if (!CMySQLDBManager::GetInstance()->UserTBL_QuerySelect()) return false;

	UserInfo* user = nullptr;
	while (!CMySQLDBManager::GetInstance()->UserTBL_Load(user))
	{
		mUser_List->push_back(user);
		user = nullptr;
	}
	return true;
}

void LoginManager::Begin()
{
	mUser_List = new vector<UserInfo*>();
	mNowLoginUser_List = new list<UserInfo*>();

	if (!UserDataLoad())
	{
		printf("\n데이터 불러오기 실패\n");
	}
}


bool LoginManager::OverLap(CClientSection* ptr)
{
	CLock lock;
	char id[128];
	ZeroMemory(id, sizeof(id));

	UnPacking(ptr->UnPackData(), id);
	int result = NODATA;
	int size = 0;
	char buf[BUFSIZE];
	ZeroMemory(buf, sizeof(buf));

	for (int i = 0; i < mUser_List->size(); i++)
	{
		if (!strcmp(mUser_List->at(i)->id, id))
		{
			result = ERROR_JOIN_EXISTS;
			size = Packing(buf, result, "이미 있는 ID 입니다.");
			break;
		}
	}

	if (result == NODATA)
	{
		size = Packing(buf, result, "사용 할 수 있는 ID 입니다.");
	}

	//이부분 수정
	unsigned __int64 full_code = NULL;
	protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::LOGIN_STATE);
	protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());
	protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::JOIN_OVERLAPRESULT);

	ptr->PackingData(full_code, buf, size);

	if (!ptr->Send()) return false;

	return true;
}

void LoginManager::End()
{
	mUser_List->clear();
	if (mUser_List != nullptr) delete mUser_List;

	mNowLoginUser_List->clear();
	if (mNowLoginUser_List != nullptr) delete mNowLoginUser_List;
}

bool LoginManager::Join(CClientSection* ptr)
{
	CLock lock;
	UserInfo temp;
	ZeroMemory(&temp, sizeof(UserInfo));
	UnPacking(ptr->UnPackData(), temp.id, temp.pw, temp.name);
	printf("temp.id = %s\n", temp.id);
	printf("temp.pw = %s\n", temp.pw);
	printf("temp.pw = %s\n", temp.name);
	int join_result = NODATA;
	int result;

	char msg[MSGSIZE];
	char buf[BUFSIZE];
	ZeroMemory(msg, sizeof(msg));
	ZeroMemory(buf, sizeof(buf));

	for (int i = 0; i < mUser_List->size(); i++)
	{
		if (!strcmp(mUser_List->at(i)->id, temp.id))
		{
			join_result = ERROR_JOIN_EXISTS;
			strcpy(msg, ERROR_JOIN_MSG);
			break;
		}
	}

	if (join_result == NODATA)
	{
		UserInfo* user = new UserInfo(temp.id, temp.pw, temp.name, (mUser_List->size()+1));

		mUser_List->push_back(user);

		//// MySQL 데이터 저장
		if (!CMySQLDBManager::GetInstance()->UseUserDB()) return false;
		CMySQLDBManager::GetInstance()->UserTBL_Insert(user);

		join_result = JOIN_SUCCESS;
		strcpy(msg, JOIN_SUCCESS_MSG);
	}

	int size = Packing(buf, join_result, msg);
	//이부분 수정
	unsigned __int64 full_code = NULL;
	protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::LOGIN_STATE);
	protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());
	protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::JOIN_RESULT);

	ptr->PackingData(full_code, buf, size);

	if (!ptr->Send()) return false;

	return true;
}

bool LoginManager::Login(CClientSection* ptr)
{
	CLock lock;
	UserInfo temp;
	ZeroMemory(&temp, sizeof(UserInfo));
	UnPacking(ptr->UnPackData(), temp.id, temp.pw);
	printf("temp.id = %s", temp.id);
	printf("temp.pw = %s\n", temp.pw);
	int login_result = NODATA;
	int result;
	int size = 0;
	char msg[MSGSIZE];
	char buf[BUFSIZE];
	ZeroMemory(msg, sizeof(msg));
	ZeroMemory(buf, sizeof(buf));

	for (int i = 0; i < mUser_List->size(); i++)
	{
		if (!strcmp(mUser_List->at(i)->id, temp.id))
		{
			if (!strcmp(mUser_List->at(i)->pw, temp.pw))
			{
				if (!mUser_List->at(i)->login)
				{
					ptr->Login(mUser_List->at(i));
					printf("real.id = %s ", temp.id);
					printf("real.num = %d\n", ptr->GetUser()->number);
					mUser_List->at(i)->login = true;

					login_result = LOGIN_SUCCESS;
					strcpy(msg, mUser_List->at(i)->name);

					printf("Login Success\n");
					// size = Packing(buf, login_result, ptr->GetUser()->number, msg);
					size = Packing(buf, login_result, msg);

					//ChatManager::GetInstance()->Add(ptr);
				}
				else
				{
					login_result = ERROR_LOGIN_EXISTS;
					strcpy(msg, ERROR_LOGIN_EXISTS_MSG);
					size = Packing(buf, login_result, msg);
				}
			}
			else
			{
				login_result = ERROR_LOGIN_PW;
				strcpy(msg, ERROR_LOGIN_PW_MSG);
				size = Packing(buf, login_result, msg);
			}
			break;
		}
	}

	if (login_result == NODATA)
	{
		login_result = ERROR_LOGIN_ID;
		strcpy(msg, ERROR_LOGIN_ID_MSG);
		size = Packing(buf, login_result,  msg);
	}
	//이부분 수정
	unsigned __int64 full_code = NULL;
	protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::LOGIN_STATE);
	protocol->ProtocolMaker(full_code, ptr->GetState()->Get_Sub_State());
	protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::LOGIN_RESULT);

	ptr->PackingData(full_code, buf, size);

	if (!ptr->Send())
	{
		return false;
	}
	return true;
}

void LoginManager::Logout(CClientSection* ptr)
{
	CLock lock;
	if (ptr->IsLogin()) ptr->Logout();
}

int LoginManager::Packing(char* buf, int result, const char* str)
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

int LoginManager::Packing(char* buf, int result, int num, const char* str)
{
	char* ptr = buf;
	int len = strlen(str);
	int size = 0;

	memcpy(ptr, &result, sizeof(result));
	ptr = ptr + sizeof(result);
	size = size + sizeof(result);

	memcpy(ptr, &num, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &len, sizeof(len));
	ptr = ptr + sizeof(len);
	size = size + sizeof(len);

	memcpy(ptr, str, len);
	ptr = ptr + len;
	size = size + len;

	return size;
}

void LoginManager::UnPacking(const char* buf, char* id)
{
	int id_len, pw_len;
	const char* ptr = buf;

	memcpy(&id_len, ptr, sizeof(id_len));
	ptr = ptr + sizeof(id_len);

	memcpy(id, ptr, id_len);
	ptr = ptr + id_len;
}

void LoginManager::UnPacking(const char* buf, char* id, char* pw)
{
	int id_len, pw_len;
	const char* ptr = buf;

	memcpy(&id_len, ptr, sizeof(id_len));
	ptr = ptr + sizeof(id_len);

	memcpy(id, ptr, id_len);
	ptr = ptr + id_len;

	memcpy(&pw_len, ptr, sizeof(pw_len));
	ptr = ptr + sizeof(pw_len);

	memcpy(pw, ptr, pw_len);
}

void LoginManager::UnPacking(const char* buf, char* id, char* pw, char* name)
{
	int id_len, pw_len, name_len;
	const char* ptr = buf;

	memcpy(&id_len, ptr, sizeof(id_len));
	ptr = ptr + sizeof(id_len);

	memcpy(id, ptr, id_len);
	ptr = ptr + id_len;

	memcpy(&pw_len, ptr, sizeof(pw_len));
	ptr = ptr + sizeof(pw_len);

	memcpy(pw, ptr, pw_len);
	ptr = ptr + pw_len;

	memcpy(&name_len, ptr, sizeof(name_len));
	ptr = ptr + sizeof(name_len);

	memcpy(name, ptr, name_len);
}