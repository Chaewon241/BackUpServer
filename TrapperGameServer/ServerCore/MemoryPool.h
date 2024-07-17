#pragma once

/// <summary>
/// 각각의 메모리풀은 각각의 크기를 가지고 있다.
/// </summary>

enum
{
	SLIST_ALIGNMENT = 16
};

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{
	// [MemoryHeader][Data]
	MemoryHeader(int32 size) : allocSize(size) {}
	
	// 메모리를 받은 후 헤더를 기입해준다.
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size);
		// 메모리 헤더 크기만큼 건너뛴다.
		return reinterpret_cast<void*>(++header);
	}
	
	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
};

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void Push(MemoryHeader* ptr);
	MemoryHeader* Pop();

private:
	SLIST_HEADER	_header;
	// 각각 자신이 담당하고 있는 메모리 사이즈
	int32 _allocSize = 0;
	// 사용중인 메모리 영역 수
	atomic<int32>	_useCount = 0;
	// 보관중인 메모리 영역 수
	atomic<int32>	_reserveCount = 0;
};
