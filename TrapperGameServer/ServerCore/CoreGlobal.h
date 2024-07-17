#pragma once

/// <summary>
/// 전역으로 사용하는 변수들을 관리하는 클래스
/// </summary>

extern class ThreadManager*		GThreadManager;
extern class Memory*			GMemory;
extern class SendBufferManager* GSendBufferManager;

extern class DeadLockProfiler*	GDeadLockProfiler;
extern class DBConnectionPool*	GDBConnectionPool;
extern class ConsoleLog*		GConsoleLogger;