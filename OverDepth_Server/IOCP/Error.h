#pragma once
#include "Global.h"

class CError 
{
private:
	DECLARE_SINGLETONE(CError)
	CError() = default;
	~CError() = default;
public:

	void err_quit(const char* msg);
	void err_display(const char* msg);
	void err_display(const int code);
};