#pragma once

extern RoomManagerRef GRoomManager;

class RoomManager
{
public:
	RoomManager();
	~RoomManager();

public:
	void Initialize();

	uint64 CreateRoom(string hostIp);

	string JoinRoom(int32 roomNum);

	void DestroyRoom(int32 roomNum);

	uint64 GenerateRoomNumber();

private:
	USE_LOCK;
	HashMap<int32, RoomRef> _rooms;
};