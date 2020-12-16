#include "MySQLDataBaseManager.h"
IMPLEMENT_SINGLETON(CMySQLDBManager)

CMySQLDBManager::CMySQLDBManager()
{
	connection = nullptr;
	ZeroMemory(&conn, sizeof(conn));
	sql_result = nullptr;
	sql_row = nullptr;
	DB_Num = 0;
}

CMySQLDBManager::~CMySQLDBManager()
{

}

void CMySQLDBManager::Begin()
{
	printf("MySQL Ver : %s\n", mysql_get_client_info());

	// 초기화
	mysql_init(&conn);

	// DB 연결
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s\n", mysql_error(&conn));
		exit(0);
	}
	else
	{
		printf("성공\n");
	}
}

bool CMySQLDBManager::UseUserDB()
{
	const char* query = "use userdb;";	// From 다음 DB에 존재하는 테이블 명으로 수정하세요
	int query_stat = mysql_query(connection, query);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return false;
	}

	mysql_set_character_set(connection, "euckr");
	return true;
}

bool CMySQLDBManager::UserTBL_QuerySelect()
{ 
	int query_stat = mysql_query(connection, "select * from usertbl;");
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return false;
	}

	UpdateSQLResult();
	return true;
}

bool CMySQLDBManager::UserTBL_Load(UserInfo*& user)
{
	if (UpdateSQLROW() != NULL)
	{
		user = new UserInfo(sql_row[ID], sql_row[PW], sql_row[NAME], ++DB_Num);
		return false;
	}
	return true;
}

void CMySQLDBManager::UserTBL_Insert(UserInfo* user)
{
	char query[MSGSIZE];

	sprintf_s(query, "insert into usertbl values('%s', '%s', '%s');", user->id, user->pw, user->name);
	
	int query_stat = mysql_query(connection, query);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return;
	}
}

void CMySQLDBManager::End()
{
	mysql_free_result(sql_result);
	mysql_close(connection);
}