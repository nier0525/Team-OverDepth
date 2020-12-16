#include "ClientSection.h"
#include "InitState.h"
#include "ChatState.h"
#include"LoginState.h"
CInitState::CInitState()
{
	mState = INIT_STATE;
}

CInitState::~CInitState()
{

}

unsigned __int64 CInitState::Get_Sub_State()
{
	return INIT_STATE;
}

void CInitState::RecvProcess(CClientSection* ptr)
{
	switch (mState)
	{
	case INIT_STATE:
		printf("Init\n");
		//다른 class의 state를 굳이 여기서 해줄 필요성은 초기 작업의 유무에 따라 달라진다. \
		  혹시 어떤 작업이 시작하기전 해야하는 작업이 있다는걸 전제하에 여기서 다른 class의 state를 지정 해줌. \
		  초기 작업이 다 이루어진 바로 직 후 바로 실행되야 하는 state를 여기서 지정해줌.
	 
		ptr->SetState(ptr->GetLoginState());
		break;
	}
}

void CInitState::SendProcess(CClientSection* ptr)
{

}