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