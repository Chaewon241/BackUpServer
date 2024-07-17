#pragma once
#include <stack>
/*
TLS(Thread Local Storage) : thread 별로 고유한 저장공간을 가질 수 있는 방법이다.
각각의 thread는 고유한 Stack을 가지기 때문에 Stack 변수(지역 변수)는 thread별로 고유하다.
그래서 각각의 thread가 같은 함수를 실행한다고 해도 그 함수에서 정의된 지역 변수는
실제로 서로 다른 메모리 공간에 위치한다는 의미다.
그러나 정적 변수와 전역 변수의 경우에는 모든 thread에 공유된다.
*/

/// <summary>///
/// 쓰레드에서 사용할 TLS를 관리하는 클래스
/// </summary>

// 생성 순서대로 쓰레드ID 부여해주기(원래는 중구난방이라 직접 부여한다.)
extern thread_local uint32 LThreadId;
extern thread_local std::stack<int32>	LLockStack;
extern thread_local SendBufferChunkRef	LSendBufferChunk;