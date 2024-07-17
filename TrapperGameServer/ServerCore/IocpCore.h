#pragma once

/// <summary>
/// IOCP에 등록할 수 있는 Object클래스
/// 세션과 비슷함
/// </summary>

class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};

/// <summary>
/// IOCP의 CP를 만드는 곳
/// </summary>

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE		GetHandle() { return _iocpHandle; }

	// 소켓이나 세션을 만들었을 때 등록을 시킨다.
	bool		Register(IocpObjectRef iocpObject);
	// worker 쓰레드에 일감이 없나 두리번 거리는 함수.
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE		_iocpHandle;
};