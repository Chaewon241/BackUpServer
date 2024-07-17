#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"

/*------------------
	ThreadManager
-------------------*/

ThreadManager::ThreadManager()
{
	// Main Thread
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	// 동시다발적으로 해줄까봐 LockGuard를 걸어준다.
	LockGuard guard(_lock);

	_threads.push_back(thread([=]()
		{
			InitTLS();
			callback();
			DestroyTLS();
		}));
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
			t.join();
	}
	_threads.clear();
}

void ThreadManager::InitTLS()
{
	// 1부터 시작하는 전역변수
	static Atomic<uint32> SThreadId = 1;
	// ID 발급기
	LThreadId = SThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{

}