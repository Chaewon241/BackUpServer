#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

IocpCore::IocpCore()
{
	// 처음에 CP 만드는거.
	_iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(_iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	::CloseHandle(_iocpHandle);
}

bool IocpCore::Register(IocpObjectRef iocpObject)
{
	// 원래는 소켓, 세션을 받아서 등록을 시켜서 관찰을 시킨다.
	// 근데 얘가 꼭 세션만 받아서 등록시키는게 아니다.
	// iocpObject->GetHandle() 얘가 소켓, 관찰 대상이다 라고 등록
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, /*key*/0, 0);
}

bool IocpCore::Dispatch(uint32 timeoutMs)
{
	// 일감이 있는지 확인
	
	// 복원을 위한 준비
	// 여기서 조심해야할 점!
	// 참조카운팅을 통해 얘가 recv send할 때 절대로 삭제되지 않게 해야한다.
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;
	IocpEvent* iocpEvent = nullptr;

	if (::GetQueuedCompletionStatus(_iocpHandle, OUT & numOfBytes, OUT & key, OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		// 성공했을 때
		// 주인님을 디스패치
		IocpObjectRef iocpObject = iocpEvent->owner;
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		// 실패했을 때 오류코드를 통해 확인하기.
		// 예를 들어서 100ms를 기다리기로 헀는데 그 때동안 일감이 없으면
		// WAIT_TIMEOUT으로 걸린다.
		int32 errCode = ::WSAGetLastError();
		switch (errCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			// TODO : 로그 찍기
			IocpObjectRef iocpObject = iocpEvent->owner;
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

	return true;
}
