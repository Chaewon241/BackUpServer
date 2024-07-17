#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class AcceptEvent;
class ServerService;

/// <summary>
/// 문지기, 안내원
/// 리스너를 생성하고 생성한 리스너를 관심 대상으로 살펴보게 함
/// IOCP에 등록할 대상
/// </summary>

class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	/* 외부에서 사용 */
	bool StartAccept(ServerServiceRef service);
	void CloseSocket();

public:
	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* 수신 관련 */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

protected:
	SOCKET _socket = INVALID_SOCKET;
	Vector<AcceptEvent*> _acceptEvents;
	ServerServiceRef _service;
};

