#include "pch.h"
#include "AccountManager.h"
#include "DBConnectionPool.h"
#include "DBBind.h"
#include "XmlParser.h"
#include "DBSynchronizer.h"
#include "GenProcedures.h"
#include "GameSession.h"
#include "DBManager.h"

#define _CRT_SECURE_NO_WARNINGS

AccountManagerRef GAccountManager;

AccountManager::AccountManager()
{
}

bool AccountManager::Join(string id, string password, string nickname)
{
	vector<DBManager::Player> players = GDBManager->get_all_player();

	// 아이디 중복 확인
	for (auto p : players)
	{
		string player;
		player.assign(p.playerId.begin(), p.playerId.end());
		if (player.compare(id) == 0)
			return false;
	}

	WRITE_LOCK;
	const size_t idLength = 50;
	WCHAR wId[idLength];
	ZeroMemory(wId, sizeof(wId));
	MultiByteToWideChar(CP_UTF8, 0, id.c_str(), -1, wId, idLength);

	const size_t nicknameLength = 50;
	WCHAR wNickname[nicknameLength];
	ZeroMemory(wNickname, sizeof(wNickname));
	MultiByteToWideChar(CP_UTF8, 0, nickname.c_str(), -1, wNickname, nicknameLength);

	const size_t passwordLength = 50;
	WCHAR wPassword[passwordLength];
	ZeroMemory(wPassword, sizeof(wPassword));
	MultiByteToWideChar(CP_UTF8, 0, password.c_str(), -1, wPassword, passwordLength);

	SP::InsertPlayer insertPlayer(*GDBManager->GetDBConn());
	insertPlayer.In_PlayerId(wId);
	insertPlayer.In_PlayerPassword(wPassword);
	insertPlayer.In_PlayerNickname(wNickname);

	if (insertPlayer.Execute())
	{
		int insertedId;
		if (insertPlayer.Fetch())
		{
			SQLGetData(GDBManager->GetDBConn()->GetStatement(), 1, SQL_C_SLONG, &insertedId, 0, nullptr);

			DBManager::Player player(insertedId, wId, wPassword, wNickname);

			GDBManager->add_player_record(player);
		}
	}

	return true;
}

bool AccountManager::Login(string id, string password)
{
	vector<DBManager::Player> players = GDBManager->get_all_player();

	for (auto p : players)
	{
		string playerId;
		playerId.assign(p.playerId.begin(), p.playerId.end());
		string playerPassword;
		playerPassword.assign(p.playerPassword.begin(), p.playerPassword.end());
		if (playerId.compare(id) == 0)
		{
			if(playerPassword.compare(password) == 0)
				return true;
			else
				return false;
		}
	}
	// 아이디가 없을 때
	return false;
}

UserInfo AccountManager::GetAccountInfo(string id)
{
	vector<DBManager::Player> players = GDBManager->get_all_player();

	for (auto p : players)
	{
		string playerId;
		playerId.assign(p.playerId.begin(), p.playerId.end());
		string playerNickname;
		playerNickname.assign(p.playerNickname.begin(), p.playerNickname.end());

		if (playerId.compare(id) == 0)
		{
			UserInfo user(p.id, playerId, playerNickname);
			return user;
		}
	}

	UserInfo nullUser;
	return nullUser;
}

int32 AccountManager::AddFriend(string myId, string friendId)
{
	SP::AddFriend addFriend(*GDBManager->GetDBConn());
	
	// 내 아이디
	wstring wMyId(myId.begin(), myId.end());
	addFriend.In_PlayerId(wMyId.c_str(), static_cast<int32>(wMyId.length()));


	// 친구 추가할 친구 아이디
	wstring wFriendId(friendId.begin(), friendId.end());
	addFriend.In_FriendId(wFriendId.c_str(), static_cast<int32>(wFriendId.length()));

	addFriend.Execute();

	SQLHSTMT stmt = GDBManager->GetDBConn()->GetStatement();
	SQLLEN indicator;
	SQLINTEGER result = 0;

	SQLBindCol(stmt, 1, SQL_C_SLONG, &result, sizeof(result), &indicator);

	SQLRETURN ret = addFriend.Fetch();

	if (SQL_SUCCEEDED(ret))
	{
		// Insert successful
		if (result == 1)
		{
			// 뒤집어서도 저장해주기
			DBManager::Friends friends1(wMyId, wFriendId);
			GDBManager->add_friends_record(friends1);
			DBManager::Friends friends2(wFriendId, wMyId);
			GDBManager->add_friends_record(friends2);
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

void AccountManager::PushActiveAccount(PacketSessionRef session, string id)
{
	auto gameSession = static_pointer_cast<GameSession>(session);
	m_ActiveAccount.insert({ id, gameSession });
}

void AccountManager::DelActiveAccount(string id)
{
	m_ActiveAccount.erase(id);
}
