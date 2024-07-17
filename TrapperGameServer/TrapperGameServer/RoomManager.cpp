#include "pch.h"
#include "RoomManager.h"
#include "Room.h"

#include <random>
#include <ctime>

RoomManagerRef GRoomManager;

RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
}

void RoomManager::Initialize()
{
}

uint64 RoomManager::CreateRoom(string hostIp)
{
	// 랜덤으로 방번호 만들어서 방 만들기
	uint64 roomNumber = GenerateRoomNumber();
	
	auto iter = _rooms.find(roomNumber);
	if (iter == _rooms.end())
	{
		RoomRef room = MakeShared<Room>();
		room->SetHostIp(hostIp);
		room->Enter();
		_rooms.insert({ roomNumber, room });
	}
	else if (iter != _rooms.end())
	{
		// todo 재귀 도는거 생각
		roomNumber = CreateRoom(hostIp);
	}

	return roomNumber;
}

string RoomManager::JoinRoom(int32 roomNum)
{
	auto iter = _rooms.find(roomNum);
	if (iter != _rooms.end())
	{
		if (iter->second->IsRoomFull())
			return "FULL";
		iter->second->Enter();
		return iter->second->GetHostIp();
	}
	else
	{
		return "";
	}
}

void RoomManager::DestroyRoom(int32 roomNum)
{
	// 호스트가 로비에서 나갔을 때나 게임이 시작됐을때
	auto iter = _rooms.find(roomNum);
	if (iter != _rooms.end())
	{
		_rooms.erase(roomNum);
	}
}

uint64 RoomManager::GenerateRoomNumber()
{
	// 현재 시간(초)을 시드로 사용하여 난수 생성기를 초기화
	std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
	// 1000에서 9999 사이의 정수를 생성하는 분포
	std::uniform_int_distribution<int> dist(1000, 9999);
	// 난수를 생성하고 반환
	return dist(rng);
}
