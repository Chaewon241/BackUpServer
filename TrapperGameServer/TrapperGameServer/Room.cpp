#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"

Room GRoom;

void Room::Enter()
{
	WRITE_LOCK;
	_playersNum++;
}

void Room::Leave(PlayerRef player)
{
	WRITE_LOCK;
	_playersNum--;
}


