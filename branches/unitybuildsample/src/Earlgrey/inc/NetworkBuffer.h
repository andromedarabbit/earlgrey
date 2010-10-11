#pragma once 

#include "ChainBuffer.hpp"
#include "StackMemoryManager.h"
#include "StlStackAllocator.hpp"

#include "numeric_cast.hpp"

namespace Earlgrey
{
	/*class NetworkBufferBase : public basic_buffer<BYTE>
	{
	public:
		const static DWORD NETWORK_BUFFER_BASE_DEFAULT_SIZE = 1024;

		explicit NetworkBufferBase(DWORD DefaultSize = NETWORK_BUFFER_BASE_DEFAULT_SIZE)
			:basic_buffer<BYTE>(DefaultSize)
		{}

		~NetworkBufferBase() {}
	};*/

	class NetworkBuffer
	{
	public:
		const static DWORD NETWORK_BUFFER_DEFAULT_SIZE = 1024;

		typedef chain_buffer<BYTE>		BufferType;
		typedef BufferType::size_type	SizeType;

		//! chain_buffer�� initial capacity �� �����Ѵ�.
		explicit NetworkBuffer(DWORD DefaultSize = NETWORK_BUFFER_DEFAULT_SIZE)
			: _ChainBuffer(DefaultSize)
		{
		}

		NetworkBuffer(const NetworkBuffer& rhs)
		{
			rhs._ChainBuffer.copy_to( _ChainBuffer );
		}

		~NetworkBuffer()
		{
			_ChainBuffer.clear();
		}

		NetworkBuffer& operator=(const NetworkBuffer& rhs)
		{
			rhs._ChainBuffer.copy_to( _ChainBuffer );
			return *this;
		}

		//! Ư�� ũ�⸸ŭ ���۸� Ȯ���� �� ���Ϲ��۸� ��´�.
		/*!
			Capacity�� Ȯ���� ������ ���۸� ���Ϲ��ۿ� �����Ѵ�.
			SendBuffer�ʹ� �޸� chain_buffer�� ������ buffer ��常�� ����Ѵ�.
		*/
		WSABUF* GetSockRecvBuffer(SizeType Size = NETWORK_BUFFER_DEFAULT_SIZE)
		{
			EARLGREY_ASSERT( Size > 0 );

			WSABUF* SockBuf = new WSABUF;

			BufferType::buffer_node_desc_type desc = _ChainBuffer.expand( Size );
			SockBuf->buf = reinterpret_cast<CHAR*>(std::tr1::get<0>( desc ));
			SockBuf->len = EARLGREY_NUMERIC_CAST<ULONG>(std::tr1::get<1>( desc ));

			return SockBuf;
		}

		//! ������ �Ϸ�Ǹ� size�� ���������ش�.
		void OnReceived(DWORD Transferred)
		{
			_ChainBuffer.increase_tail_size( Transferred );
		}

		WSABUF* GetSockSendBuffer()
		{
			WSABUF* SocketBuffer = new WSABUF[_ChainBuffer.chain_size()];//! TODO : shared_ptr?

			chain_buffer<BYTE>::desc_vector_type desc_vector;
			_ChainBuffer.get_descriptions( desc_vector );

			DWORD i = 0;
			chain_buffer<BYTE>::desc_vector_type::const_iterator it = desc_vector.begin();
			for(; it != desc_vector.end(); it++, i++)
			{
				SocketBuffer[i].buf = reinterpret_cast<CHAR*>(std::tr1::get<0>( *it ));
				SocketBuffer[i].len = EARLGREY_NUMERIC_CAST<ULONG>(std::tr1::get<1>( *it ));
			}

			return SocketBuffer;
		}

		DWORD GetBufferSize() const
		{
			return EARLGREY_NUMERIC_CAST<DWORD>(_ChainBuffer.size());
		}

		DWORD GetBufferCapacity() const
		{
			return EARLGREY_NUMERIC_CAST<DWORD>(_ChainBuffer.capacity());
		}

		void Erase(chain_buffer<BYTE>::size_type Size)
		{
			Size;
		}

		void Clear()
		{
			_ChainBuffer.clear();
		}

		BOOL SetValue(const BYTE* InValue, DWORD InSize)
		{
			_ChainBuffer.set( InValue, InSize );
			return TRUE;
		}

		BOOL GetValue(DWORD& Offset, BYTE* OutValue, DWORD InSize)
		{
			bool result = _ChainBuffer.get( static_cast<size_t>(Offset), OutValue, InSize );
			Offset += InSize;
			return result;
		}

		// TODO �ӽ�?
		LPCTSTR ToString()
		{
			return _T("");
		}

	private:
		BufferType _ChainBuffer;
	};
}
