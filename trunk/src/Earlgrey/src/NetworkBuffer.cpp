#include "stdafx.h"
#include "NetworkBuffer.h"

namespace Earlgrey {


	NetworkBuffer::NetworkBuffer( DWORD DefaultSize /*= NETWORK_BUFFER_DEFAULT_SIZE*/ ) 
		: _ChainBuffer(DefaultSize), _SentSize(0)
	{

	}

	NetworkBuffer::NetworkBuffer( const NetworkBuffer& rhs )
	{
		rhs._ChainBuffer.copy_to( _ChainBuffer );
	}

	NetworkBuffer& NetworkBuffer::operator=( const NetworkBuffer& rhs )
	{
		rhs._ChainBuffer.copy_to( _ChainBuffer );
		return *this;
	}

	std::pair<WSABUF*, DWORD> NetworkBuffer::GetSockRecvBuffer( SizeType Size /*= NETWORK_BUFFER_DEFAULT_SIZE*/ )
	{
		EARLGREY_ASSERT( Size > 0 );

		BufferType::desc_vector_type desc = _ChainBuffer.expand( Size );
		BufferType::desc_vector_type::const_iterator iter = desc.begin();
		EARLGREY_ASSERT( desc.size() > 0 );

		WSABUF* SockBuf = new WSABUF[desc.size()];

		DWORD i = 0;
		for (; iter != desc.end(); iter++, i++)
		{
			SockBuf[i].buf = reinterpret_cast<CHAR*>(std::tr1::get<0>( *iter ));
			SockBuf[i].len = EARLGREY_NUMERIC_CAST<ULONG>(std::tr1::get<1>( *iter ));
		}			

		return std::make_pair( SockBuf, i );
	}

	void NetworkBuffer::OnReceived( DWORD Transferred )
	{
		_ChainBuffer.increase_size( Transferred );
	}

	std::pair<WSABUF*, DWORD> NetworkBuffer::GetSockSendBuffer()
	{
		WSABUF* SocketBuffer = new WSABUF[_ChainBuffer.chain_size()];

		chain_buffer<BYTE>::desc_vector_type desc_vector;
		_ChainBuffer.get_descriptions( _SentSize, desc_vector );

		DWORD i = 0;
		chain_buffer<BYTE>::desc_vector_type::const_iterator it = desc_vector.begin();
		for(; it != desc_vector.end(); it++, i++)
		{
			SocketBuffer[i].buf = reinterpret_cast<CHAR*>(std::tr1::get<0>( *it ));
			SocketBuffer[i].len = EARLGREY_NUMERIC_CAST<ULONG>(std::tr1::get<1>( *it ));
		}

		return std::make_pair( SocketBuffer, EARLGREY_NUMERIC_CAST<DWORD>(desc_vector.size()) );
	}

	size_t NetworkBuffer::GetBufferSize() const
	{
		return EARLGREY_NUMERIC_CAST<DWORD>(_ChainBuffer.size());
	}

	size_t NetworkBuffer::GetBufferCapacity() const
	{
		return EARLGREY_NUMERIC_CAST<DWORD>(_ChainBuffer.capacity());
	}

	void NetworkBuffer::Erase( chain_buffer<BYTE>::size_type Size )
	{
		Size;
	}

	void NetworkBuffer::Clear()
	{
		_ChainBuffer.clear();
	}

	BOOL NetworkBuffer::SetValue( size_t Offset, const BYTE* InValue, size_t Size )
	{
		_ChainBuffer.set( Offset, InValue, Size );
		return TRUE;
	}

	BOOL NetworkBuffer::GetValue(size_t Offset, BYTE* OutValue, size_t Size)
	{
		return _ChainBuffer.get( Offset, OutValue, Size );
	}

	LPCTSTR NetworkBuffer::ToString()
	{
		return _T("");
	}

	size_t NetworkBuffer::Shrink(size_t ValidIndex)
	{
		size_t bufSize = 0, shrunk = 0;
		for (;;)
		{
			BufferType::buffer_type* buffer = _ChainBuffer.front_buffer();
			bufSize = buffer->size();
			if (shrunk + bufSize > ValidIndex)
			{
				break;
			}
			_ChainBuffer.pop_front_buffer();
			shrunk += bufSize;
		}

		return shrunk;
	}

	NetworkBuffer::~NetworkBuffer()
	{
		_ChainBuffer.clear();
	}

	NetworkBuffer::SharedPtr NetworkBuffer::Create()
	{
		return make_ptr(new NetworkBuffer());
	}

	void NetworkBuffer::OnSent( DWORD Transferred )
	{
		_SentSize += Transferred;
	}

	bool NetworkBuffer::SentCompleted() const
	{
		return _SentSize == _ChainBuffer.size();
	}

	void* NetworkBuffer::operator new( size_t size )
	{
		UNREFERENCED_PARAMETER(size);
		StlDefaultAllocator<AsyncResult>::Type allocator;
		void* p = allocator.allocate(1);
		if (!p)
		{
			throw std::bad_alloc();
		}
		return p;
	}

	void* NetworkBuffer::operator new[]( size_t size )
	{
		UNREFERENCED_PARAMETER(size);
		StlDefaultAllocator<AsyncResult>::Type allocator;
		void* p = allocator.allocate(1);
		if (!p)
		{
			throw std::bad_alloc();
		}
		return p;
	}

	void NetworkBuffer::AppendValue( const BYTE* InValue, size_t Size )
	{
		_ChainBuffer.append( InValue, Size );
	}

	void NetworkBuffer::operator delete[]( void* p )
	{
		StlDefaultAllocator<AsyncResult>::Type allocator;
		return allocator.deallocate(reinterpret_cast<AsyncResult*>(p), 0);
	}

	void NetworkBuffer::operator delete( void* p )
	{
		StlDefaultAllocator<AsyncResult>::Type allocator;
		return allocator.deallocate(reinterpret_cast<AsyncResult*>(p), 0);
	}

}