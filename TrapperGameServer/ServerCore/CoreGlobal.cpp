#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "DeadLockProfiler.h"
#include "SocketUtils.h"
#include "SendBuffer.h"
#include "DBConnectionPool.h"
#include "ConsoleLog.h"

// 전역으로 사용할 매니저
ThreadManager*		GThreadManager = nullptr;
Memory*				GMemory = nullptr;
SendBufferManager*	GSendBufferManager = nullptr;

DeadLockProfiler*	GDeadLockProfiler = nullptr;
DBConnectionPool*	GDBConnectionPool = nullptr;
ConsoleLog*			GConsoleLogger = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		// 생성하고 소멸하는 순서가 정해져야할 수도 있어서 
		// 컨텐츠(main)에서 생성하는게 아니라 생성자에서 생성해준다.
		GThreadManager = new ThreadManager();
		GMemory = new Memory();
		GSendBufferManager = new SendBufferManager();
		GDeadLockProfiler = new DeadLockProfiler();
		GDBConnectionPool = new DBConnectionPool();
		GConsoleLogger = new ConsoleLog();

		// 윈속 초기화
		SocketUtils::Init();
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemory;
		delete GSendBufferManager;
		delete GDeadLockProfiler;
		delete GDBConnectionPool;
		delete GConsoleLogger;

		SocketUtils::Clear();
	}
}GCoreGlobal;
// 전역객체로 만들기