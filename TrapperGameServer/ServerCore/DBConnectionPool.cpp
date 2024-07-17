#include "pch.h"
#include "DBConnectionPool.h"

DBConnectionPool::DBConnectionPool()
{

}

DBConnectionPool::~DBConnectionPool()
{
	Clear();
}

bool DBConnectionPool::Connect(int32 connectionCount, const WCHAR* connectionString)
{
	// enviroment라는 환경을 만들어서 어떤 버전을 사용할 것인지 연동해준다
	WRITE_LOCK;

	// SQL_HANDLE_ENV(ENV는 enviroment)
	if (::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_environment) != SQL_SUCCESS)
		return false;

	if (::SQLSetEnvAttr(_environment, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0) != SQL_SUCCESS)
		return false;

	for (int32 i = 0; i < connectionCount; i++)
	{
		DBConnection* connection = xnew<DBConnection>();
		// connectionString 과 enviroment를 넘겨준다.
		if (connection->Connect(_environment, connectionString) == false)
			return false;

		_connections.push_back(connection);
	}

	return true;
}

void DBConnectionPool::Clear()
{
	WRITE_LOCK;

	if (_environment != SQL_NULL_HANDLE)
	{
		::SQLFreeHandle(SQL_HANDLE_ENV, _environment);
		_environment = SQL_NULL_HANDLE;
	}

	for (DBConnection* connection : _connections)
		xdelete(connection);

	_connections.clear();
}

DBConnection* DBConnectionPool::Pop()
{
	WRITE_LOCK;

	if (_connections.empty())
		return nullptr;

	DBConnection* connection = _connections.back();
	_connections.pop_back();
	return connection;
}

void DBConnectionPool::Push(DBConnection* connection)
{
	WRITE_LOCK;
	_connections.push_back(connection);
}
