#pragma once

#include <thread>
// std::function을 사용하기 위함.
#include <functional>

/// <summary>
/// 
/// </summary>

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	// callback 함수를 받아줌과 동시에 쓰레드를 실행시켜준다.
	void	Launch(function<void(void)> callback);
	// 쓰레드 끝날때까지 기다려주는 join
	void	Join();

	// TLS 영역을 초기화하고 파괴시켜주는 ThreadManager의 핵심부분
	static void InitTLS();
	static void DestroyTLS();

private:
	Mutex			_lock;
	vector<thread>	_threads;
};

