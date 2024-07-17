#pragma once
#include <sql.h>
#include <sqlext.h>

enum
{
	// 경계선
	WVARCHAR_MAX = 4000,
	BINARY_MAX = 8000
};

class DBConnection
{
public:
	// 실제로 연결
	bool			Connect(SQLHENV henv, const WCHAR* connectionString);
	void			Clear();

	// 쿼리를 실행하는 함수
	bool			Execute(const WCHAR* query);
	// 실행을 하고 결과를 받아오는 함수
	bool			Fetch();
	// 데이터가 몇개가 있는지
	int32			GetRowCount();
	void			Unbind();

	SQLHSTMT		GetStatement() { return _statement; };

public:
	bool			BindParam(int32 paramIndex, bool* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, float* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, double* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int8* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int16* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int32* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, int64* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
	bool			BindParam(int32 paramIndex, const WCHAR* str, SQLLEN* index);
	bool			BindParam(int32 paramIndex, const BYTE* bin, int32 size, SQLLEN* index);

	bool			BindCol(int32 columnIndex, bool* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, float* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, double* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, int8* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, int16* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, int32* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, int64* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
	bool			BindCol(int32 columnIndex, WCHAR* str, int32 size, SQLLEN* index);
	bool			BindCol(int32 columnIndex, BYTE* bin, int32 size, SQLLEN* index);

private:
	// SQL 쿼리를 실행할 때 인자를 넘겨줄 때가 있는데 statement를 통해서 보내줘야한다.
	bool			BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	bool			BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
	void			HandleError(SQLRETURN ret);

private:
	// 데이터베이스의 커넥션을 담당하는 핸들
	SQLHDBC			_connection = SQL_NULL_HANDLE;
	// 상태를 관리하는 핸들
	SQLHSTMT		_statement = SQL_NULL_HANDLE;
};

