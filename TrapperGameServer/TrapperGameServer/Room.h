#pragma once

class Room
{
public:
	void Enter();
	void Leave(PlayerRef player);

	void SetHostIp(string hostIp) { _hostIp = hostIp; }
	string& GetHostIp() { return _hostIp ; }

	bool IsRoomFull() { return _playersNum == 2 ? true : false; }

private:
	USE_LOCK;
	uint64 _playersNum = 0;
	string _hostIp = "";
};

extern Room GRoom;