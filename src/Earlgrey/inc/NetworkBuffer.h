#pragma once 

#include "ChainBuffer.hpp"
#include "StackMemoryManager.h"
#include "StlStackAllocator.hpp"

#include "numeric_cast.hpp"

namespace Earlgrey
{
	class NetworkBuffer 
	{
	public:
		// new/delete 연산자 오버로딩 default allocator 를 사용하도록 한다.
		static void* operator new(size_t size);
		static void operator delete(void* p);
		static void* operator new[](size_t size);
		static void operator delete[](void* p);

	public:
		const static DWORD NETWORK_BUFFER_DEFAULT_SIZE = 1024;

		typedef chain_buffer<BYTE>		BufferType;
		typedef BufferType::size_type	SizeType;
		typedef std::tr1::shared_ptr<NetworkBuffer> SharedPtr;

		//! chain_buffer의 initial capacity 를 설정한다.
		explicit NetworkBuffer(DWORD DefaultSize = NETWORK_BUFFER_DEFAULT_SIZE);
		NetworkBuffer(const NetworkBuffer& rhs);
		~NetworkBuffer();

		NetworkBuffer& operator=(const NetworkBuffer& rhs);

		//! 특정 크기만큼 버퍼를 확장한 후 소켓버퍼를 얻는다.
		std::pair<WSABUF*, DWORD> GetSockRecvBuffer(SizeType Size = NETWORK_BUFFER_DEFAULT_SIZE);

		//! 버퍼에 기록된 내용을 WSABUF 배열 형태로 가져온다.
		std::pair<WSABUF*, DWORD> GetSockSendBuffer();

		//! 수신이 완료되면 size를 증가시켜준다.
		void OnReceived(DWORD Transferred);

		//! 전송이 완료되면 전송된 크기를 누적시킨다.
		void OnSent(DWORD Transferred);

		//! 전송이 완료됐는지 검사한다.
		bool SentCompleted() const;

		size_t GetBufferSize() const;

		size_t GetBufferCapacity() const;

		void Erase(chain_buffer<BYTE>::size_type Size);

		void Clear();

		BOOL SetValue(size_t Offset, const BYTE* InValue, size_t Size);
		void AppendValue(const BYTE* InValue, size_t Size);

		BOOL GetValue(size_t Offset, BYTE* OutValue, size_t Size);

		// TODO 임시?
		LPCTSTR ToString();

		//! 사용하지 않는 버퍼 노드를 제거한다. 
		/*
			읽기용 버퍼일 경우에는 이미 읽는 버퍼를 더 이상 유지할 필요가 없다.

			\param ValidIndex 유효한 버퍼의 시작 인덱스

			\return 줄어든 버퍼의 크기
		*/
		size_t Shrink(size_t ValidIndex);

		static SharedPtr Create();

	private:
		BufferType _ChainBuffer;
		size_t _SentSize;
	};

	typedef NetworkBuffer::SharedPtr NetworkBufferPtr;
}
