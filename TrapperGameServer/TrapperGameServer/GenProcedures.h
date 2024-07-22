#pragma once
#include "Types.h"
#include <windows.h>
#include "DBBind.h"

namespace SP
{
	
    class InsertPlayer : public DBBind<3,0>
    {
    public:
    	InsertPlayer(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spInsertPlayer(?,?,?)}") { }
    	template<int32 N> void In_PlayerId(WCHAR(&v)[N]) { BindParam(0, v); };
    	template<int32 N> void In_PlayerId(const WCHAR(&v)[N]) { BindParam(0, v); };
    	void In_PlayerId(WCHAR* v, int32 count) { BindParam(0, v, count); };
    	void In_PlayerId(const WCHAR* v, int32 count) { BindParam(0, v, count); };
    	template<int32 N> void In_PlayerPassword(WCHAR(&v)[N]) { BindParam(1, v); };
    	template<int32 N> void In_PlayerPassword(const WCHAR(&v)[N]) { BindParam(1, v); };
    	void In_PlayerPassword(WCHAR* v, int32 count) { BindParam(1, v, count); };
    	void In_PlayerPassword(const WCHAR* v, int32 count) { BindParam(1, v, count); };
    	template<int32 N> void In_PlayerNickname(WCHAR(&v)[N]) { BindParam(2, v); };
    	template<int32 N> void In_PlayerNickname(const WCHAR(&v)[N]) { BindParam(2, v); };
    	void In_PlayerNickname(WCHAR* v, int32 count) { BindParam(2, v, count); };
    	void In_PlayerNickname(const WCHAR* v, int32 count) { BindParam(2, v, count); };

    private:
    };

    class GetPlayerInfo : public DBBind<1,0>
    {
    public:
    	GetPlayerInfo(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spGetPlayerInfo(?)}") { }
    	template<int32 N> void In_PlayerId(WCHAR(&v)[N]) { BindParam(0, v); };
    	template<int32 N> void In_PlayerId(const WCHAR(&v)[N]) { BindParam(0, v); };
    	void In_PlayerId(WCHAR* v, int32 count) { BindParam(0, v, count); };
    	void In_PlayerId(const WCHAR* v, int32 count) { BindParam(0, v, count); };

    private:
    };

    class CheckPlayerIdExists : public DBBind<1,0>
    {
    public:
    	CheckPlayerIdExists(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spCheckPlayerIdExists(?)}") { }
    	template<int32 N> void In_PlayerId(WCHAR(&v)[N]) { BindParam(0, v); };
    	template<int32 N> void In_PlayerId(const WCHAR(&v)[N]) { BindParam(0, v); };
    	void In_PlayerId(WCHAR* v, int32 count) { BindParam(0, v, count); };
    	void In_PlayerId(const WCHAR* v, int32 count) { BindParam(0, v, count); };

    private:
    };

    class LoginPlayer : public DBBind<2,0>
    {
    public:
    	LoginPlayer(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spLoginPlayer(?,?)}") { }
    	template<int32 N> void In_PlayerId(WCHAR(&v)[N]) { BindParam(0, v); };
    	template<int32 N> void In_PlayerId(const WCHAR(&v)[N]) { BindParam(0, v); };
    	void In_PlayerId(WCHAR* v, int32 count) { BindParam(0, v, count); };
    	void In_PlayerId(const WCHAR* v, int32 count) { BindParam(0, v, count); };
    	template<int32 N> void In_PlayerPassword(WCHAR(&v)[N]) { BindParam(1, v); };
    	template<int32 N> void In_PlayerPassword(const WCHAR(&v)[N]) { BindParam(1, v); };
    	void In_PlayerPassword(WCHAR* v, int32 count) { BindParam(1, v, count); };
    	void In_PlayerPassword(const WCHAR* v, int32 count) { BindParam(1, v, count); };

    private:
    };

    class DeleteAllGold : public DBBind<0,0>
    {
    public:
    	DeleteAllGold(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spDeleteAllGold}") { }

    private:
    };

    class GetAllPlayers : public DBBind<0,0>
    {
    public:
    	GetAllPlayers(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spGetAllPlayers}") { }

    private:
    };

    class DeleteAllPlayersAndFriends : public DBBind<0,0>
    {
    public:
    	DeleteAllPlayersAndFriends(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spDeleteAllPlayersAndFriends}") { }

    private:
    };

    class AddFriend : public DBBind<2,0>
    {
    public:
    	AddFriend(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spAddFriend(?,?)}") { }
    	template<int32 N> void In_PlayerId(WCHAR(&v)[N]) { BindParam(0, v); };
    	template<int32 N> void In_PlayerId(const WCHAR(&v)[N]) { BindParam(0, v); };
    	void In_PlayerId(WCHAR* v, int32 count) { BindParam(0, v, count); };
    	void In_PlayerId(const WCHAR* v, int32 count) { BindParam(0, v, count); };
    	template<int32 N> void In_FriendId(WCHAR(&v)[N]) { BindParam(1, v); };
    	template<int32 N> void In_FriendId(const WCHAR(&v)[N]) { BindParam(1, v); };
    	void In_FriendId(WCHAR* v, int32 count) { BindParam(1, v, count); };
    	void In_FriendId(const WCHAR* v, int32 count) { BindParam(1, v, count); };

    private:
    };

    class RemoveFriend : public DBBind<2,0>
    {
    public:
    	RemoveFriend(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spRemoveFriend(?,?)}") { }
    	template<int32 N> void In_UserId(WCHAR(&v)[N]) { BindParam(0, v); };
    	template<int32 N> void In_UserId(const WCHAR(&v)[N]) { BindParam(0, v); };
    	void In_UserId(WCHAR* v, int32 count) { BindParam(0, v, count); };
    	void In_UserId(const WCHAR* v, int32 count) { BindParam(0, v, count); };
    	template<int32 N> void In_FriendId(WCHAR(&v)[N]) { BindParam(1, v); };
    	template<int32 N> void In_FriendId(const WCHAR(&v)[N]) { BindParam(1, v); };
    	void In_FriendId(WCHAR* v, int32 count) { BindParam(1, v, count); };
    	void In_FriendId(const WCHAR* v, int32 count) { BindParam(1, v, count); };

    private:
    };

    class GetFriendsList : public DBBind<1,0>
    {
    public:
    	GetFriendsList(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spGetFriendsList(?)}") { }
    	template<int32 N> void In_PlayerId(WCHAR(&v)[N]) { BindParam(0, v); };
    	template<int32 N> void In_PlayerId(const WCHAR(&v)[N]) { BindParam(0, v); };
    	void In_PlayerId(WCHAR* v, int32 count) { BindParam(0, v, count); };
    	void In_PlayerId(const WCHAR* v, int32 count) { BindParam(0, v, count); };

    private:
    };

    class GetAllFriends : public DBBind<0,0>
    {
    public:
    	GetAllFriends(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spGetAllFriends}") { }

    private:
    };


     
};