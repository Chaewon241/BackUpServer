#pragma once

class SendBufferChunk;

/// <summary>
/// send 할 때마다 보내지 않고 묶어서 보내기 위해
/// </summary>

class SendBuffer
{
public:
	SendBuffer(SendBufferChunkRef owner, BYTE* buffer, int32 allocSize);
	~SendBuffer();

	BYTE*		Buffer() { return _buffer; }
	uint32		AllocSize() { return _allocSize; }
	int32		WriteSize() { return _writeSize; }
	void		Close(uint32 writeSize);

private:
	BYTE*				_buffer;
	uint32				_allocSize = 0;
	uint32				_writeSize = 0;
	SendBufferChunkRef	_owner;
};

/*--------------------
	SendBufferChunk
--------------------*/

/// <summary>
/// Buffer들을 필요할 때마다 만드는 것이 아닌 큰 덩어리를 만들어서
/// 잘라서 쓸 수 있게 하는 버퍼
/// </summary>

class SendBufferChunk : public enable_shared_from_this<SendBufferChunk>
{
	enum
	{
		SEND_BUFFER_CHUNK_SIZE = 6000
	};

public:
	SendBufferChunk();
	~SendBufferChunk();

	void				Reset();
	SendBufferRef		Open(uint32 allocSize);
	void				Close(uint32 writeSize);

	bool				IsOpen() { return _open; }
	BYTE*				Buffer() { return &_buffer[_usedSize]; }
	uint32				FreeSize() { return static_cast<uint32>(_buffer.size()) - _usedSize; }

private:
	Array<BYTE, SEND_BUFFER_CHUNK_SIZE>		_buffer = {};
	bool									_open = false;
	uint32									_usedSize = 0;
};

/*---------------------
	SendBufferManager
----------------------*/

/// <summary> 
/// SendBuffer의 Push/Pop 기능을 제공
/// </summary>

class SendBufferManager
{
public:
	SendBufferRef		Open(uint32 size);

private:
	SendBufferChunkRef	Pop();
	void				Push(SendBufferChunkRef buffer);

	static void			PushGlobal(SendBufferChunk* buffer);

private:
	USE_LOCK;
	Vector<SendBufferChunkRef> _sendBufferChunks;
};
