#include "pch.h"
#include "ServerPacketHandler.h"
#include "ClientContent.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// 직접 컨텐츠 작업자

void ProcessLogin(PacketSessionRef& session)
{
	Protocol::C_LOGIN loginPkt;
	cout << "id를 입력하세여 : ";
	string id;
	cin >> id;
	loginPkt.set_id(id);
	cout << "비밀번호를 입력하세여 : ";
	string password;
	cin >> password;
	loginPkt.set_password(password);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);
}

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_S_CREATE_ACCOUNT(PacketSessionRef& session, Protocol::S_CREATE_ACCOUNT& pkt)
{
	if (pkt.success() == false)
	{
		cout << "회원가입 실패" << endl;
	}
	else
	{
		cout << "회원가입 성공" << endl;
	}
	return true;
	
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success() == false)
	{
		cout << "로그인 실패" << endl;
	}
	else
	{
		GClientContent->ChangeRunning();
		cout << "로그인 성공" << endl;
	}

	return true;
}

bool Handle_S_ADD_FRIEND(PacketSessionRef& session, Protocol::S_ADD_FRIEND& pkt)
{
	auto result = pkt.success();

	if (result == 1)
		cout << "친구추가 성공" << endl;
	else if (result == 2)
		cout << "유저아이디랑 친구아이디랑 똑같으면 안됨" << endl;
	else if (result == 3)
		cout << "친구아이디가 없음" << endl;
	else if (result == 4)
		cout << "이미 친구임" << endl;
	else if (result == 5)
		cout << "친구추가 실패" << endl;

	return false;
}

bool Handle_S_GET_FRIEND(PacketSessionRef& session, Protocol::S_GET_FRIEND& pkt)
{
	return false;
}


bool Handle_S_CREATE_ROOM(PacketSessionRef& session, Protocol::S_CREATE_ROOM& pkt)
{
	return false;
}

bool Handle_S_JOIN_ROOM(PacketSessionRef& session, Protocol::S_JOIN_ROOM& pkt)
{
	return false;
}

bool Handle_S_DESTROY_ROOM(PacketSessionRef& session, Protocol::S_DESTROY_ROOM& pkt)
{
	return false;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	// TODO
	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	std::cout << pkt.msg() << endl;
	return true;
}
