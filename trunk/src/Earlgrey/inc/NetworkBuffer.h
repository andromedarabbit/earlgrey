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

		void Initialize()
		{
		}

		CHAR* GetAcceptBuffer()
		{
			chain_buffer<BYTE>::buffer_node_desc_type ret = _ChainBuffer.expand( NETWORK_BUFFER_DEFAULT_SIZE );

			return reinterpret_cast<CHAR*>(std::tr1::get<0>( ret ));
		}

		WSABUF* GetSockRecvBuffer()
		{
			WSABUF* SocketBuffer = new WSABUF; //! TODO : shared_ptr?
			chain_buffer<BYTE>::buffer_node_desc_type ret = _ChainBuffer.expand( NETWORK_BUFFER_DEFAULT_SIZE );

			SocketBuffer->buf = reinterpret_cast<CHAR*>(std::tr1::get<0>( ret ));
			SocketBuffer->len = Math::numeric_cast<ULONG>(std::tr1::get<1>( ret ));

			return SocketBuffer;
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
				SocketBuffer[i].len = Math::numeric_cast<ULONG>(std::tr1::get<1>( *it ));
			}

			return SocketBuffer;
		}

		DWORD GetBufferSize()
		{
			return (DWORD)_ChainBuffer.size();
		}

		DWORD GetBufferCapacity()
		{
			return (DWORD)_ChainBuffer.capacity();
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

		//юс╫ц?
		LPCTSTR ToString()
		{
			return _T("");
		}

	private:
		chain_buffer<BYTE> _ChainBuffer;
	};
}
