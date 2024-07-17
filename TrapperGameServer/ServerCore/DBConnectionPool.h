#pragma once
#include "DBConnection.h"

class DBConnectionPool
{
public:
	DBConnectionPool();
	~DBConnectionPool();

	// 커넥션 풀을 만들어주는 함수
	bool					Connect(int32 connectionCount, const WCHAR* connectionString);
	void					Clear();

	DBConnection*			Pop();
	void					Push(DBConnection* connection);

private:
	USE_LOCK;
	// 환경을 담당하는 핸들
	SQLHENV					_environment = SQL_NULL_HANDLE;
	// 연결을 여러개를 만들고 재사용하기 위해서 vector로 들고 있는다.
	Vector<DBConnection*>	_connections;
};

