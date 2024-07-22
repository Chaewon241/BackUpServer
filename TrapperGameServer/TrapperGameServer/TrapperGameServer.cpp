#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include <tchar.h>
#include "Protocol.pb.h"
#include "DBConnectionPool.h"
#include "DBBind.h"
#include "XmlParser.h"
#include "DBSynchronizer.h"
#include "GenProcedures.h"
#include "AccountManager.h"
#include "RoomManager.h"
#include "DBManager.h"

#define Login

std::wstring convert_to_wstring(const std::string& str) 
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
	return wstr;
}

int main(int argc, char* argv[])
{
	/*ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={ODBC Driver 17 for SQL Server};Server=(localdb)\\MSSQLLocalDB;Database=ServerDb;Trusted_Connection=yes;"));

	DBConnection* dbConn = GDBConnectionPool->Pop();
	DBSynchronizer dbSync(*dbConn);
	dbSync.Synchronize(L"GameDB.xml");

	SP::DeleteAllGold deleteAllGold(*dbConn);
	deleteAllGold.Execute();*/

	std::vector<std::wstring> args;
	for (int i = 0; i < argc; ++i) 
	{
		args.push_back(convert_to_wstring(argv[i]));
	}

	ClientPacketHandler::Init();

#ifdef Login
	GAccountManager = MakeShared<AccountManager>();
	GDBManager = MakeShared<DBManager>();
	GDBManager->Initialize();
#endif
	GRoomManager = MakeShared<RoomManager>();

	// todo 채원 ip 바꾸기
	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(args[1], 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>, // TODO : SessionManager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 64; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	/*while (true)
	{
		Protocol::S_CHAT pkt;
		pkt.set_msg("HelloWorld");
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);

		GSessionManager.Broadcast(sendBuffer);
		this_thread::sleep_for(1s);
	}*/

	GThreadManager->Join();
}
