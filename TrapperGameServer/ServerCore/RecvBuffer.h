#pragma once

/// <summary>
/// readpos와 writepos를 통해 recvbuffer의 데이터를 읽고 쓴다.
/// </summary>

// [r][][w][][][][][][][][][]
// [][][][][][][][][][][][]

class RecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer();

	void			Clean();
	bool			OnRead(int32 numOfBytes);
	bool			OnWrite(int32 numOfBytes);
	 
	BYTE*			ReadPos() { return &_buffer[_readPos]; }
	BYTE*			WritePos() { return &_buffer[_writePos]; }
	int32			DataSize() { return _writePos - _readPos; }
	int32			FreeSize() { return _capacity - _writePos; }

private:
	int32			_capacity = 0;
	int32			_bufferSize = 0;
	// 현재 읽는 위치
	int32			_readPos = 0;
	// 현재 쓰는 위치
	int32			_writePos = 0;
	// 실질적으로 데이터를 저장하는 곳
	Vector<BYTE>	_buffer;
};

