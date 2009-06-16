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

	class NetworkBuffer : Uncopyable
	{
	public:
		const static DWORD NETWORK_BUFFER_DEFAULT_SIZE = 1024;
		explicit NetworkBuffer(DWORD DefaultSize = NETWORK_BUFFER_DEFAULT_SIZE)
			: _ChainBuffer(DefaultSize)
		{
			
		}

		~NetworkBuffer()
		{
			_ChainBuffer.clear();
		}

		void Initialize()
		{
			basic_buffer<BYTE> newBuffer(_ChainBuffer.capacity());
			//newBuffer.reserve(_ChainBuffer.capacity());
			BYTE tempStr = 'a';
			newBuffer.set(&tempStr , 1);
			_ChainBuffer.set(&newBuffer, 1);
		}

		DWORD GetBufferSize()
		{
			return (DWORD)_ChainBuffer.chain_size();
		}

		WSABUF* GetSockRecvBuffer()
		{
			WSABUF* SocketBuffer = new WSABUF; //! TODO : shared_ptr?

			SocketBuffer->buf = (CHAR*)_ChainBuffer.front().data();
			SocketBuffer->len = (ULONG)_ChainBuffer.front().capacity();

			return SocketBuffer;
		}

		WSABUF* GetSockSendBuffer()
		{
			WSABUF* SocketBuffer = new WSABUF;//! TODO : shared_ptr?

			//chain_buffer<basic_buffer<BYTE>>::iterator it = _ChainBuffer.begin();
			DWORD i = 0;
			//for(; it != _ChainBuffer.end(); it++)
			{
				SocketBuffer[i].buf = (CHAR*)_ChainBuffer.front().data();//(CHAR*)it->front()/*.data()*/;//((*it).data()/*.front()*/);
				SocketBuffer[i].len = (ULONG)_ChainBuffer.front().size();//(*it).size();
				i++;
			}

			return SocketBuffer;
		}

		BOOL SetValue(BYTE* InValue, DWORD InSize)
		{
			basic_buffer<BYTE>* tmp = new basic_buffer<BYTE>(InSize);
			tmp->set(InValue, InSize);
			_ChainBuffer.set(tmp, InSize);

			return TRUE;
		}

		//юс╫ц?
		BYTE* ToString()
		{
			return (BYTE*)_ChainBuffer.front().data();
		}

	private:
		chain_buffer<basic_buffer<BYTE>> _ChainBuffer;
	};
}
