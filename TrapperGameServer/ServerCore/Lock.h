#pragma once
#include "Types.h"

/// <summary>
/// Reader Writer SpinLock
/// 32비트 변수를 사용. 
/// 상위 16비트는 WriteFlag -> Lock을 획득하고 있는 쓰레드의 ID
/// 하위 16비트는 ReadFlag -> 공유해서 사용하고 있는 Read Count
/// </summary>


// W -> W (O) 같은 쓰레드 내에서 Write할 때 Write 가능. 근데 다른 스레드는 안댐.
// W -> R (O) Write한 상태에서 Read 가능
// R -> W (X)
// 

class Lock
{
	enum : uint32
	{
		// 최대로 기다려줄 Tick(시간)
		ACQUIRE_TIMEOUT_TICK = 10000,
		// SpinCount을 최대 몇번 돌지
		MAX_SPIN_COUNT = 5000,
		// Flag
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};
public:
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);

private:
	Atomic<uint32> _lockFlag;
	// 독단적으로 사용하는것이라 경합이 붙지 않음.
	uint16 _writeCount = 0;
};

/*---------------
	LockGuards
----------------*/

class ReadLockGuard
{
public:
	// 객체가 만들어질때 Lock해줌
	ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.ReadLock(name); }
	// 객체가 소멸될때 UnLock해줌
	~ReadLockGuard() { _lock.ReadUnlock(_name); }

private:
	Lock& _lock;
	const char* _name;
};

class WriteLockGuard
{
public:
	// 객체가 만들어질때 Lock해줌
	WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.WriteLock(name); }
	// 객체가 소멸될때 UnLock해줌
	~WriteLockGuard() { _lock.WriteUnlock(_name); }

private:
	Lock& _lock;
	const char* _name;
};