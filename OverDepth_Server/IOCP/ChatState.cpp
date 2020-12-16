#include "ChatState.h"
ChatState::ChatState()
{
	mState = STATE::RECV_STATE;
}

unsigned __int64 ChatState::Get_Sub_State()
{
	return (unsigned __int64)mState;
}

void ChatState::RecvProcess(CClientSection* ptr)
{

	unsigned __int64 full_code = ptr->GetProtocol();
	printf("ptr->GetProtocol()\n");
	//이 부분 수정 클라이언트의 main state 확인
	bool code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, (unsigned __int64)CLASS_STATE::CHAT_STATE,NULL,NULL);
	if (code_check == TRUE)
	{
		printf("(unsigned __int64)CLASS_STATE::CHAT_STATE\n");
		//이 부분 수정 클라이언트의 sub state 확인
		code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code, NULL,(unsigned __int64)STATE::RECV_STATE,NULL);
		if (code_check == TRUE)
		{
			printf("STATE::RECV_STATE\n");
			//이 부분 수정 클라이언트의 protocol 확인
			code_check = CProtocol::GetInstance()->ProtocolUnpacker(full_code,NULL,NULL, (unsigned __int64)ChatManager::PROTOCOL::RECV);
			if (code_check == TRUE)
			{
				printf(" ChatManager::PROTOCOL::RECV\n");
				ChatManager::GetInstance()->Recv_Message(ptr);
				mState = STATE::RECV_STATE;
				return;
			}
		}
	}
	else
	{
		return;
	}

}

void ChatState::SendProcess(CClientSection* ptr)
{
	switch (mState)
	{
	case ChatState::STATE::SEND_STATE:
		mState = STATE::RECV_STATE;
		break;
	}
}
