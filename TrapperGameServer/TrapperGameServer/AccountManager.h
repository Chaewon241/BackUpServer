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
};

class AccountManager
{
public:
	AccountManager();

public:
	bool Join(string id, string password, string nickname);
	bool Login(string id, string password);

	UserInfo& GetAccountInfo(string id);

	int32 AddFriend(string id);
	vector<tuple<int32, string, string>>& GetFriends(string id);

	void PushActiveAccount(PacketSessionRef session, string id);
	void DelActiveAccount(string id);

private:
	USE_LOCK;
	unordered_map<string, GameSessionRef> m_ActiveAccount;
	DBConnection* m_DbConn = nullptr;
	UserInfo m_MyInfo = {};
};

