#include "pch.h"
#include "DBManager.h"
#include "DBConnectionPool.h"
#include "DBBind.h"
#include "XmlParser.h"
#include "DBSynchronizer.h"
#include "GenProcedures.h"
#include "Protocol.pb.h"

DBManagerRef GDBManager;

// Player테이블에 있는 정보들 다 가져오기.
void DBManager::Initializeplayer()
{
	SP::GetAllPlayers getPlayers(*m_DbConn);

	getPlayers.Execute();

	SQLHSTMT stmt = m_DbConn->GetStatement();
	SQLSMALLINT uid;
	SQLWCHAR playerId[50];
	SQLWCHAR playerPassword[50];
	SQLWCHAR playerNickname[50];
	SQLLEN uidLen;
	SQLLEN playerIdLen;
	SQLLEN playerPasswordLen;
	SQLLEN playerNicknameLen;

	SQLBindCol(stmt, 1, SQL_C_SSHORT, &uid, sizeof(uid), &uidLen);
	SQLBindCol(stmt, 2, SQL_C_WCHAR, playerId, sizeof(playerId), &playerIdLen);
	SQLBindCol(stmt, 3, SQL_C_WCHAR, playerPassword, sizeof(playerPasswordLen), &playerPasswordLen);
	SQLBindCol(stmt, 4, SQL_C_WCHAR, playerNickname, sizeof(playerNickname), &playerNicknameLen);


	while (getPlayers.Fetch())
	{
		int32 convertUid = (int32)uid;
		wstring convertId = std::wstring(playerId, playerIdLen / sizeof(SQLWCHAR));
		wstring convertPassword = std::wstring(playerPassword, playerPasswordLen / sizeof(SQLWCHAR));
		wstring convertNickname = std::wstring(playerNickname, playerNicknameLen / sizeof(SQLWCHAR));

		Player player(convertUid, convertId, convertPassword, convertNickname);

		add_player_record(player);
	}
}

// Friends테이블에 있는 정보들 다 가져오기.
void DBManager::Initializefriends()
{
	SP::GetAllFriends getFriends(*m_DbConn);

	getFriends.Execute();

	SQLHSTMT stmt = m_DbConn->GetStatement();
	SQLWCHAR playerId[50];
	SQLWCHAR friendId[50];
	SQLLEN playerIdLen;
	SQLLEN friendIdLen;

	SQLBindCol(stmt, 1, SQL_C_WCHAR, playerId, sizeof(playerId), &playerIdLen);
	SQLBindCol(stmt, 2, SQL_C_WCHAR, friendId, sizeof(friendIdLen), &friendIdLen);

	while (getFriends.Fetch())
	{
		wstring convertplayerId = std::wstring(playerId, playerIdLen / sizeof(SQLWCHAR));
		wstring convertfriendId = std::wstring(friendId, friendIdLen / sizeof(SQLWCHAR));

		Friends friends(convertplayerId, convertfriendId);

		add_friends_record(friends);
	}
}

void DBManager::Initialize()
{
	ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={ODBC Driver 17 for SQL Server};Server=(localdb)\\MSSQLLocalDB;Database=ServerDb;Trusted_Connection=yes;"));

	m_DbConn = GDBConnectionPool->Pop();
	DBSynchronizer dbSync(*m_DbConn);
	dbSync.Synchronize(L"GameDB.xml");

	Initializeplayer();
	Initializefriends();
}
