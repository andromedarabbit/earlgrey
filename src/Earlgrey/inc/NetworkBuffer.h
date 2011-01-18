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
		// new/delete ������ �����ε� default allocator �� ����ϵ��� �Ѵ�.
		static void* operator new(size_t size);
		static void operator delete(void* p);
		static void* operator new[](size_t size);
		static void operator delete[](void* p);

	public:
		const static DWORD NETWORK_BUFFER_DEFAULT_SIZE = 1024;

		typedef chain_buffer<BYTE>		BufferType;
		typedef BufferType::size_type	SizeType;
		typedef std::tr1::shared_ptr<NetworkBuffer> SharedPtr;

		//! chain_buffer�� initial capacity �� �����Ѵ�.
		explicit NetworkBuffer(DWORD DefaultSize = NETWORK_BUFFER_DEFAULT_SIZE);
		NetworkBuffer(const NetworkBuffer& rhs);
		~NetworkBuffer();

		NetworkBuffer& operator=(const NetworkBuffer& rhs);

		//! Ư�� ũ�⸸ŭ ���۸� Ȯ���� �� ���Ϲ��۸� ��´�.
		std::pair<WSABUF*, DWORD> GetSockRecvBuffer(SizeType Size = NETWORK_BUFFER_DEFAULT_SIZE);

		//! ���ۿ� ��ϵ� ������ WSABUF �迭 ���·� �����´�.
		std::pair<WSABUF*, DWORD> GetSockSendBuffer();

		//! ������ �Ϸ�Ǹ� size�� ���������ش�.
		void OnReceived(DWORD Transferred);

		//! ������ �Ϸ�Ǹ� ���۵� ũ�⸦ ������Ų��.
		void OnSent(DWORD Transferred);

		//! ������ �Ϸ�ƴ��� �˻��Ѵ�.
		bool SentCompleted() const;

		size_t GetBufferSize() const;

		size_t GetBufferCapacity() const;

		void Erase(chain_buffer<BYTE>::size_type Size);

		void Clear();

		BOOL SetValue(const BYTE* InValue, size_t Size);

		BOOL GetValue(size_t Offset, BYTE* OutValue, size_t Size);

		// TODO �ӽ�?
		LPCTSTR ToString();

		//! ������� �ʴ� ���� ��带 �����Ѵ�. 
		/*
			�б�� ������ ��쿡�� �̹� �д� ���۸� �� �̻� ������ �ʿ䰡 ����.

			\param ValidIndex ��ȿ�� ������ ���� �ε���

			\return �پ�� ������ ũ��
		*/
		size_t Shrink(size_t ValidIndex);

		static SharedPtr Create();

	private:
		BufferType _ChainBuffer;
		size_t _SentSize;
	};

	typedef NetworkBuffer::SharedPtr NetworkBufferPtr;
}
