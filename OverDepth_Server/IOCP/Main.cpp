#include "CtrlHandler.h"
#include "MySQLDataBaseManager.h"

int main()
{
	CCtrlHandler handler;
	handler.Begin();

	if (CServerManager::GetInstance()->Begin())
	{
		CServerManager::GetInstance()->Run();
	}

	return 0;
}
