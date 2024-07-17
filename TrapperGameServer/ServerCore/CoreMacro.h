#pragma once

// 출력값
#define OUT

// namespace 설정해주기
#define NAMESPACE_BEGIN(name)	namespace name {
#define NAMESPACE_END			}

/*----------
	Lock
-----------*/
#define USE_MANY_LOCKS(count)	Lock _locks[count];
#define USE_LOCK				USE_MANY_LOCKS(1)
#define	READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name());
#define READ_LOCK				READ_LOCK_IDX(0)
#define	WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(_locks[idx], typeid(this).name());
#define WRITE_LOCK				WRITE_LOCK_IDX(0)

/*-----------
	Memory
------------*/
#ifdef _DEBUG
#define xAlloc(size)	PoolAllocator::Alloc(size)
#define xRelease(ptr)	PoolAllocator::Release(ptr)
#else
#define xAlloc(size)	BaseAllocator::Alloc(size)
#define xRelease(ptr)	BaseAllocator::Release(ptr)
#endif

/*-----------
	Crash
------------*/

// 인위적으로 크래쉬내기
// complie 단계에서 잡아버리지 않게 하기 위해 매크로 사용
// 운빨로 크래쉬 만들지 말고 인위적으로 만들어서 확인해보자.
//

// CRASH(입력)
#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

// __analysis_assume(crash != nullptr); -> crash는 nullptr이 아니라고 먼저 해주기
// crash가 nullptr이면 미리 컴파일러가 잡아줄 수 있기 때문.
// 

#define ASSERT_CRASH(expr)			\
{									\
	if (!(expr))					\
	{								\
		CRASH("ASSERT_CRASH");		\
		__analysis_assume(expr);	\
	}								\
}
