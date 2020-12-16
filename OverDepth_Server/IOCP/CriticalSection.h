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
