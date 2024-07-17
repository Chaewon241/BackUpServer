#include "pch.h"
#include "Memory.h"
#include "MemoryPool.h"

Memory::Memory()
{
	int32 size = 0;
	int32 tableIndex = 0;

	// 1024까지는 32바이트씩 메모리풀 만들기.
	for (size = 32; size <= 1024; size += 32)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		// 0 ~ 32 바이트까지는 이 테이블을 참고해라.
		while (tableIndex <= size)
		{
			_poolTable[tableIndex] = pool;
			tableIndex++;
		}
	}

	for (; size <= 2048; size += 128)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIndex <= size)
		{
			_poolTable[tableIndex] = pool;
			tableIndex++;
		}
	}

	for (; size <= 4096; size += 256)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIndex <= size)
		{
			_poolTable[tableIndex] = pool;
			tableIndex++;
		}
	}
}

Memory::~Memory()
{
	for (MemoryPool* pool : _pools)
		delete pool;

	_pools.clear();
}

void* Memory::Allocate(int32 size)
{
	MemoryHeader* header = nullptr;
	const int32 allocSize = size + sizeof(MemoryHeader);

#ifdef _STOMP
	header = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(allocSize));
#else
	if (allocSize > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링 최대 크기를 벗어나면 일반 할당
	//	// 기존에 만든 StompAllocator랑은 개념이 다름. 
	//	// 왜냐하면 StompAllocator는 메모리 공간이 필요없어지면 아예 메모리를 해제시키는데
	//	// 얘는 아예 메모리를 해제시키는 것이 아닌 임시 저장공간에 저장해두는 것이라서 다르다.
		header = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(allocSize, SLIST_ALIGNMENT));
	}
	else
	{
		// 메모리 풀에서 꺼내온다
		header = _poolTable[allocSize]->Pop();
	}
#endif	

	return MemoryHeader::AttachHeader(header, allocSize);
}

void Memory::Release(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

	const int32 allocSize = header->allocSize;
	ASSERT_CRASH(allocSize > 0);


#ifdef _STOMP
	StompAllocator::Release(header);
#else
	if (allocSize > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링 최대 크기를 벗어나면 일반 해제
		::_aligned_free(header);
	}
	else
	{
		// 메모리 풀에 반납한다
		_poolTable[allocSize]->Push(header);
	}
#endif	
}
