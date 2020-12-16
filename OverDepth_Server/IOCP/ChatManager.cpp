#include "ChatManager.h"
IMPLEMENT_SINGLETON(ChatManager)

ChatManager::ChatManager()
{
    protocol = CProtocol::GetInstance();
}

void ChatManager::UnPacking(const char* buf, char* msg)
{
	int msg_len;
	const char* ptr = buf;

	memcpy(&msg_len, ptr, sizeof(msg_len));
	ptr = ptr + sizeof(msg_len);

	memcpy(msg, ptr, msg_len);
	ptr = ptr + msg_len;
}

int ChatManager::Packing(char* buf, const char* str)
{
	char* ptr = buf;
	int len = strlen(str);
	int size = 0;

	memcpy(ptr, &len, sizeof(len));
	ptr = ptr + sizeof(len);
	size = size + sizeof(len);

	memcpy(ptr, str, len);
	ptr = ptr + len;
	size = size + len;

	return size;
}

void ChatManager::Begin()
{
	chat_user = new list< CClientSection*>();
}

void ChatManager::End()
{
	chat_user->clear();
	if (chat_user != nullptr)
	{
		delete chat_user;
	}
}

void ChatManager::Add(CClientSection* ptr)
{
	printf("ChatManager Add\n");
	chat_user->push_back(ptr);
}

void ChatManager::Remove(CClientSection* ptr)
{
	CLock lock;
	printf("ChatManager Remove\n");
	chat_user->remove(ptr);
}

bool ChatManager::Recv_Message(CClientSection* ptr)
{
	CLock lock;
	char msg[MSGSIZE];
	char buf[BUFSIZE];
	ZeroMemory(msg, sizeof(msg));
	ZeroMemory(buf, sizeof(buf));
	UnPacking(ptr->UnPackData(), msg);
	printf("msg = %s\n", msg);

	int size = Packing(buf, msg);
	//이부분 수정
	unsigned __int64 full_code = NULL;
	protocol->ProtocolMaker(full_code, (unsigned __int64)CLASS_STATE::CHAT_STATE);
	std::cout << std::hex << full_code << std::endl;

	protocol->ProtocolMaker(full_code,	ptr->GetState()->Get_Sub_State());
	std::cout << std::hex << full_code << std::endl;

	protocol->ProtocolMaker(full_code, (unsigned __int64)PROTOCOL::RECV);
	std::cout << std::hex << full_code << std::endl;

	list< CClientSection*>::iterator iter;
	cout << chat_user->size() << endl;

	for (iter = chat_user->begin(); iter != chat_user->end(); iter++)
	{
		((CClientSection*)*iter)->PackingData(full_code, buf, size);
		if (!((CClientSection*)*iter)->Send())
		{
			printf("IOOOUT");
			return false;
		}
	}	
}

bool ChatManager::Send_Message(CClientSection* ptr)
{
	return false;
}
