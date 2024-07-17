#pragma once

// -------------------
//		1차 시도
// -------------------
/*
struct SListEntry
{
	SListEntry* next;
};

struct SListHeader
{
	SListEntry* next = nullptr;
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);
*/

// -------------------
//		2차 시도
// -------------------

//struct SListEntry
//{
//	SListEntry* next;
//};
//
//struct SListHeader
//{
//	SListEntry* next = nullptr;
//};
//
//void InitializeHead(SListHeader* header);
// Push랑 Pop이 다중쓰레드에서 잘 작동되게 바꾸기
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//SListEntry* PopEntrySList(SListHeader* header);

// -------------------
//		3차 시도
// -------------------
// 무조건 메모리가 16바이트로 정렬되게 해달라
DECLSPEC_ALIGN(16)
struct SListEntry
{
	SListEntry* next;
};

DECLSPEC_ALIGN(16)
struct SListHeader
{
	SListHeader()
	{
		alignment = 0;
		region = 0;
	}

	// 안에 있는 구조체를 같은 데이터로 취급
	// 크게 접근하냐(위에 구조체) 세부적으로 접근하냐(아래 구조체)
	union
	{
		struct
		{
			uint64 alignment;
			uint64 region;
		} DUMMYSTRUCTNAME;
		struct
		{
			// 얘네들 다 합하면 위에랑 똑같이 128
			uint64 depth : 16;
			uint64 sequence : 48;
			uint64 reserved : 4;
			uint64 next : 60;
		} HeaderX64;
	};
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);