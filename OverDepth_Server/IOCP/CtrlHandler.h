#pragma once
#include "ServerManager.h"

class CCtrlHandler
{
private:
	bool success;
public:
	CCtrlHandler()
	{

	}

	~CCtrlHandler() {}

	 static bool CtrlHandler(DWORD CtrlType)
	{
		switch (CtrlType)
		{
		case CTRL_C_EVENT:
		case CTRL_CLOSE_EVENT:
		case CTRL_BREAK_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT:
		default:
			// Function
			CServerManager::GetInstance()->End();
			printf("Exit\n");
			return false;
		}
	}

	void Begin()
	{
		success = SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, true);
		if (!success)
		{
			printf("Could Not Set Control Handler");
			exit(0);
		}
	}
};