#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 allocSize): _allocSize(allocSize)
{
	::InitializeSListHead(&_header);
}

MemoryPool::~MemoryPool()
{
	// _aligned_malloc으로 만들었으면 _aligned_free로 해줘야한다.
	// 메모리가 null이 아니면~
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
		::_aligned_free(memory);
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	ptr->allocSize = 0;

	// Pool에 메모리 반납
	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr));

	_useCount.fetch_sub(1);
	_reserveCount.fetch_add(1);
}

MemoryHeader* MemoryPool::Pop()
{
	// 원래는 여분이 있는지 확인하고 있으면 새로 만들어주는 방식이었는데
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));

	// 없으면 새로 만들다
	if (memory == nullptr)
	{
		// _aligned_malloc으로 원하는 정렬된 malloc을 할 수 있다.
		memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(_allocSize, SLIST_ALIGNMENT));
	}
	else
	{
		ASSERT_CRASH(memory->allocSize == 0);
		_reserveCount.fetch_sub(1);
	}

	_useCount.fetch_add(1);

	return memory;
}
