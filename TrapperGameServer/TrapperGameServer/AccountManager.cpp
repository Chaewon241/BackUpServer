#include "pch.h"
#include "AccountManager.h"
#include "DBConnectionPool.h"
#include "DBBind.h"
#include "XmlParser.h"
#include "DBSynchronizer.h"
#include "GenProcedures.h"
#include "GameSession.h"

#define _CRT_SECURE_NO_WARNINGS

AccountManagerRef GAccountManager;

AccountManager::AccountManager()
{
	ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={ODBC Driver 17 for SQL Server};Server=(localdb)\\MSSQLLocalDB;Database=ServerDb;Trusted_Connection=yes;"));

	m_DbConn = GDBConnectionPool->Pop();
	DBSynchronizer dbSync(*m_DbConn);
	dbSync.Synchronize(L"GameDB.xml");
}

bool AccountManager::Join(string id, string password, string nickname)
{
	WRITE_LOCK;

	const size_t idLength = 50;
	WCHAR wId[idLength];
	ZeroMemory(wId, sizeof(wId));
	MultiByteToWideChar(CP_UTF8, 0, id.c_str(), -1, wId, idLength);

	SP::CheckPlayerIdExists checkPlayer(*m_DbConn);
	checkPlayer.In_PlayerId(wId);

	checkPlayer.Execute();

	SQLHSTMT stmt = m_DbConn->GetStatement();
	SQLLEN indicator;
	SQLSMALLINT exists = 0;
	SQLRETURN ret = checkPlayer.Fetch();

	if (SQL_SUCCEEDED(ret)) 
	{
		ret = SQLGetData(stmt, 1, SQL_C_SSHORT, &exists, 0, &indicator);
		if (SQL_SUCCEEDED(ret)) 
		{
			if (exists) 
			{
				return false;
			}
		}
	}

	const size_t nicknameLength = 50;
	WCHAR wNickname[nicknameLength];
	ZeroMemory(wNickname, sizeof(wNickname));
	MultiByteToWideChar(CP_UTF8, 0, nickname.c_str(), -1, wNickname, nicknameLength);

	const size_t passwordLength = 50;
	WCHAR wPassword[passwordLength];
	ZeroMemory(wPassword, sizeof(wPassword));

	MultiByteToWideChar(CP_UTF8, 0, password.c_str(), -1, wPassword, passwordLength);

	SP::InsertPlayer insertPlayer(*m_DbConn);
	insertPlayer.In_PlayerId(wId);
	insertPlayer.In_PlayerPassword(wPassword);
	insertPlayer.In_PlayerNickname(wNickname);

	insertPlayer.Execute();

	return true;
}

bool AccountManager::Login(string id, string password)
{
	SP::LoginPlayer loginPlayer(*m_DbConn);

	const size_t idLength = 50;
	WCHAR wId[idLength];
	ZeroMemory(wId, sizeof(wId));
	MultiByteToWideChar(CP_UTF8, 0, id.c_str(), -1, wId, idLength);
	loginPlayer.In_PlayerId(wId);

	const size_t passwordLength = 50;
	WCHAR wPassword[passwordLength];
	ZeroMemory(wPassword, sizeof(wPassword));
	MultiByteToWideChar(CP_UTF8, 0, password.c_str(), -1, wPassword, passwordLength);
	loginPlayer.In_PlayerPassword(wPassword);

	loginPlayer.Execute();

	SQLHSTMT stmt = m_DbConn->GetStatement();
	SQLLEN indicator;
	SQLSMALLINT exists = 0;
	SQLRETURN ret = loginPlayer.Fetch();

	if (SQL_SUCCEEDED(ret))
	{
		ret = SQLGetData(stmt, 1, SQL_C_SSHORT, &exists, 0, &indicator);
		if (SQL_SUCCEEDED(ret))
		{
			// 비밀번호 틀림
			if (exists == 0)
			{
				return false;
			}
			// 로그인 성공
			else if (exists == 1)
			{
				return true;
			}
			// ID 존재하지 않음
			else if (exists == 2)
			{
				return false;
			}
		}
	}

	return true;
}

UserInfo& AccountManager::GetAccountInfo(string id)
{
	SP::GetPlayerInfo getPlayerInfo(*m_DbConn);

	const size_t idLength = 50;
	WCHAR wId[idLength];
	ZeroMemory(wId, sizeof(wId));
	MultiByteToWideChar(CP_UTF8, 0, id.c_str(), -1, wId, idLength);
	getPlayerInfo.In_PlayerId(wId);

	/*SQLWCHAR nickname[50];
	getPlayerInfo.Out_PlayerNickname(nickname);*/

	getPlayerInfo.Execute();

	SQLHSTMT stmt = m_DbConn->GetStatement();
	
	SQLLEN indicator;

	SQLRETURN ret = getPlayerInfo.Fetch();

	if (SQL_SUCCEEDED(ret))
	{

		//std::cout << "Nickname: " << nickname << std::endl;

	}
	m_MyInfo.uid = 0;
	m_MyInfo.id = id;
	//m_MyInfo.nickname = (const char*)nickname;

	return m_MyInfo;
}

int32 AccountManager::AddFriend(string friendId)
{
	SP::AddFriend addFriend(*m_DbConn);
	// 친구 추가할 친구 아이디
	const size_t idLength = 50;
	WCHAR wFreindId[idLength];
	ZeroMemory(wFreindId, sizeof(wFreindId));
	MultiByteToWideChar(CP_UTF8, 0, friendId.c_str(), -1, wFreindId, idLength);
	addFriend.In_FriendId(wFreindId);

	// 내 아이디
	WCHAR wMyId[idLength];
	ZeroMemory(wMyId, sizeof(wMyId));
	MultiByteToWideChar(CP_UTF8, 0, m_MyInfo.id.c_str(), -1, wMyId, idLength);
	addFriend.In_PlayerId(wMyId);

	addFriend.Execute();

	SQLHSTMT stmt = m_DbConn->GetStatement();
	SQLLEN indicator;
	SQLINTEGER result = 0;

	SQLBindCol(stmt, 1, SQL_C_SLONG, &result, sizeof(result), &indicator);

	SQLRETURN ret = addFriend.Fetch();

	if (SQL_SUCCEEDED(ret))
	{
		// Insert successful
		if (result == 1)
		{
		}
		// User and friend are same
		else if (result == 2)
		{
		}
		// Friend does not exist
		else if (result == 3)
		{
		}
		// Friendship already exists
		else if (result == 4)
		{
		}
		// Insert failed
		else if (result == 5)
		{
		}
		// Unknown error code
		else
		{
		}
		return result;
	}
	// Fetch failed
	return -1;
}

vector<tuple<int32, string, string>>& AccountManager::GetFriends(string id)
{	
	SP::GetFriendsList getFriends(*m_DbConn);
	const size_t idLength = 50;
	WCHAR wId[idLength];
	ZeroMemory(wId, sizeof(wId));
	MultiByteToWideChar(CP_UTF8, 0, id.c_str(), -1, wId, idLength);
	getFriends.In_UserId(wId);
	
	WCHAR friendId[51];
	WCHAR userId[51];

	getFriends.Out_UserId(userId);
	getFriends.Out_FriendId(friendId);

	getFriends.Execute();

	SQLRETURN ret;

	vector<tuple<int32, string, string>> friends;

	while (getFriends.Fetch())
	{
		GConsoleLogger->WriteStdOut(Color::BLUE,
			L"my[%s] fri[%s] \n", userId, friendId);
	}

	return friends;
}

void AccountManager::PushActiveAccount(PacketSessionRef session, string id)
{
	auto gameSession = static_pointer_cast<GameSession>(session);
	m_ActiveAccount.insert({ id, gameSession });
}

void AccountManager::DelActiveAccount(string id)
{
	m_ActiveAccount.erase(id);
}
