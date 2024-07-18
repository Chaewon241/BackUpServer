#include "pch.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Room.h"
#include "RoomManager.h"
#include "GameSession.h"

#include "DBConnectionPool.h"
#include "DBBind.h"
#include "DBSynchronizer.h"
#include "GenProcedures.h"
#include "AccountManager.h"

#define _CRT_SECURE_NO_WARNINGS
#pragma warnings(disable: 4996)

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// 직접 컨텐츠 작업자

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_CREATE_ACCOUNT(PacketSessionRef& session, Protocol::C_CREATE_ACCOUNT& pkt)
{
	string sid = pkt.id();
	string spassword = pkt.password();
	string snickname = pkt.nickname();

	Protocol::S_CREATE_ACCOUNT createPkt;

	if (GAccountManager->Join(sid, spassword, snickname))
		createPkt.set_success(true);
	else
		createPkt.set_success(false);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(createPkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	// TODO : Validation 체크

	Protocol::S_LOGIN loginPkt;

	if(GAccountManager->Login(pkt.id(), pkt.password()))
	{
		GAccountManager->PushActiveAccount(session, pkt.id());
		UserInfo info = GAccountManager->GetAccountInfo(pkt.id());
		loginPkt.set_success(true);
		Protocol::UserInfo* userInfo = loginPkt.mutable_user();
		userInfo->set_id(info.uid);
		userInfo->set_playerid(pkt.id());
		userInfo->set_nickname(info.nickname);
	}
	else
		loginPkt.set_success(false);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_ADD_FRIEND(PacketSessionRef& session, Protocol::C_ADD_FRIEND& pkt)
{
	int32 addFriendResult = GAccountManager->AddFriend(pkt.id());

	Protocol::S_ADD_FRIEND addFriendPkt;
	addFriendPkt.set_success(addFriendResult);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(addFriendPkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_GET_FRIEND(PacketSessionRef& session, Protocol::C_GET_FRIEND& pkt)
{
	GAccountManager->GetFriends(pkt.id());

	/*vector<wstring> friends = GAccountManager->GetFriends(pkt.id());

	Protocol::S_GET_FRIEND getFriendPkt;

	for (auto f : friends)
	{
		Protocol::UserInfo* userInfo = getFriendPkt.mutable_user();
		userInfo->set_id(info.uid);
		userInfo->set_playerid(pkt.id());
		userInfo->set_nickname(info.nickname);
	}*/
	
	return false;
}

bool Handle_C_CREATE_ROOM(PacketSessionRef& session, Protocol::C_CREATE_ROOM& pkt)
{
	Protocol::S_CREATE_ROOM createRoomPkt;
	createRoomPkt.set_roomnum(GRoomManager->CreateRoom(pkt.ip()));
	createRoomPkt.set_success(true);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(createRoomPkt);
	session->Send(sendBuffer);
	return true;
}

bool Handle_C_JOIN_ROOM(PacketSessionRef& session, Protocol::C_JOIN_ROOM& pkt)
{
	Protocol::S_JOIN_ROOM joinPkt;
	string tmpIp = GRoomManager->JoinRoom(pkt.roomnum());
	
	if (tmpIp.compare("") == 0 || tmpIp.compare("FULL") == 0)
	{
		// Room이 없거나 다 찼을 때
		joinPkt.set_success(false);
		joinPkt.set_roomnum(0);
	}
	else
	{
		joinPkt.set_success(true);
		joinPkt.set_roomnum(pkt.roomnum());
	}
	joinPkt.set_ip(tmpIp);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(joinPkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_DESTROY_ROOM(PacketSessionRef& session, Protocol::C_DESTROY_ROOM& pkt)
{
	GRoomManager->DestroyRoom(pkt.roomnum());

	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	uint64 index = pkt.playerindex();
	// TODO : Validation

	PlayerRef player = gameSession->_players[index]; // READ_ONLY?
	//GRoom.Enter(player); // WRITE_LOCK

	Protocol::S_ENTER_GAME enterGamePkt;
	enterGamePkt.set_success(true);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePkt);
	player->ownerSession->Send(sendBuffer);

	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	std::cout << pkt.msg() << endl;

	Protocol::S_CHAT chatPkt;
	chatPkt.set_msg(pkt.msg());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	return true;
}
