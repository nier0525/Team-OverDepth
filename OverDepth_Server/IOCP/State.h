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