#include "pch.h"
#include "ServerPacketHandler.h"
#include "ClientContent.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// ���� ������ �۾���

void ProcessLogin(PacketSessionRef& session)
{
	Protocol::C_LOGIN loginPkt;
	cout << "id�� �Է��ϼ��� : ";
	string id;
	cin >> id;
	loginPkt.set_id(id);
	cout << "��й�ȣ�� �Է��ϼ��� : ";
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
		cout << "ȸ������ ����" << endl;
	}
	else
	{
		cout << "ȸ������ ����" << endl;
	}
	return true;
	
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success() == false)
	{
		cout << "�α��� ����" << endl;
	}
	else
	{
		GClientContent->ChangeRunning();
		cout << "�α��� ����" << endl;
	}

	return true;
}

bool Handle_S_ADD_FRIEND(PacketSessionRef& session, Protocol::S_ADD_FRIEND& pkt)
{
	auto result = pkt.success();

	if (result == 1)
		cout << "ģ���߰� ����" << endl;
	else if (result == 2)
		cout << "�������̵�� ģ�����̵�� �Ȱ����� �ȵ�" << endl;
	else if (result == 3)
		cout << "ģ�����̵� ����" << endl;
	else if (result == 4)
		cout << "�̹� ģ����" << endl;
	else if (result == 5)
		cout << "ģ���߰� ����" << endl;

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
