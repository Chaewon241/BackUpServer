#pragma once
#include <unordered_map>

USING_SHARED_PTR(AccountManager);
extern AccountManagerRef GAccountManager;

class DBConnection;

struct UserInfo 
{
	int uid;
	string id;
	string nickname;

	UserInfo() :
		uid(0),
		id(""),
		nickname("")
	{ }

	UserInfo(
		int uid,
		std::string playerId,
		std::string playerNickname
	) :
		uid(uid),
		id(playerId),
		nickname(playerNickname)
	{ }
};

class AccountManager
{
public:
	AccountManager();

public:
	bool Join(string id, string password, string nickname);
	bool Login(string id, string password);

	UserInfo GetAccountInfo(string id);

	int32 AddFriend(string myId, string friendId);
	
	void PushActiveAccount(PacketSessionRef session, string id);
	void DelActiveAccount(string id);

private:
	USE_LOCK;
	unordered_map<string, GameSessionRef> m_ActiveAccount;
};

