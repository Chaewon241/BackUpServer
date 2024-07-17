#pragma once
#include "Allocator.h"

/// <summary>
/// 메모리를 관리하는 곳.
/// 사실상 Memory Manager
/// </summary>

class MemoryPool;

class Memory
{
	enum
	{
		// 메모리가 작은 애들은 촘촘하게,,
		// ~1024까지 32단위, ~2048까지 128단위, ~4096까지 256단위
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	Memory();
	~Memory();

	void*	Allocate(int32 size);
	void	Release(void* ptr);

private:
	// 메모리 풀을 여러개 가지고 있을거니까 벡터로 가지고 있자.
	vector<MemoryPool*> _pools;

	// 메모리 크기 <-> 메모리 풀
	// O(1) 빠르게 찾기 위한 테이블
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};

template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xAlloc(sizeof(Type)));
	
	//placement new
	new(memory)Type(forward<Args>(args)...);

	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	xRelease(obj);
}

// 기본적으로 new delete를 사용하기 때문에 이걸 사용하면 안됨.
template<typename Type, typename... Args>
shared_ptr<Type> MakeShared(Args&&... args)
{
	return shared_ptr<Type>{ xnew<Type>(forward<Args>(args)...), xdelete<Type> };
}