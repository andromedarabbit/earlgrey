#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	//! C++은 int, long 등 정수형의 크기와 포맷이 플랫폼이나 컴파일러 구현 등에 맡긴다.
	// 따라서 <inttypes.h> 등을 이용해 플랫폼 간의 상이함을 해결해야 하나 VC++엔 현재 <inttype.h>가 없다.
	template <typename BufferT>
	class BinaryWriter : private Uncopyable
	{
	public:
		typedef typename BufferT::size_type size_type;


		explicit BinaryWriter(BufferT& buffer);
		~BinaryWriter();


		inline BOOL WriteBoolean(const bool x)
		{
			return WriteNumber(x);
		}
		inline BOOL WriteBoolean(const BOOL x)
		{
			return WriteNumber(x);
		}


		inline BOOL WriteByte(const BYTE x)
		{
			return WriteBytes(x, sizeof(x));
		}
		BOOL WriteBytes(const void* buf, size_type len);

		inline BOOL WriteChar(const CHAR x)
		{
			return WriteBytes(&x, sizeof(CHAR));
		}
		inline BOOL WriteChars(const CHAR* x, size_type len)
		{
			const size_type bytes = len * sizeof(CHAR);
			return WriteBytes(x, bytes);
		}

		inline BOOL WriteWChar(const WCHAR x)
		{
			return WriteBytes(&x, sizeof(WCHAR));
		}

		inline BOOL WriteWChars(const WCHAR* x, size_type len)
		{
			const size_type bytes = len * sizeof(WCHAR);
			return WriteBytes(x, bytes);
		}


		inline BOOL WriteDouble(const DOUBLE x)
		{
			return WriteNumber(x);
		}
		inline BOOL WriteFloat(const FLOAT x)
		{
			return WriteNumber(x);
		}


		inline BOOL WriteInt8(const INT8 x)
		{
			return WriteNumber(x);
		}
		inline BOOL WriteInt16(const INT16 x)
		{
			return WriteNumber(x);
		}
		inline BOOL WriteInt32(const INT32 x)
		{
			return WriteNumber(x);
		}
		inline BOOL WriteInt64(const INT64 x)
		{
			return WriteNumber(x);
		}

		inline BOOL WriteUInt8(const UINT8 x)
		{
			return WriteNumber(x);
		}
		inline BOOL WriteUInt16(const UINT16 x)
		{
			return WriteNumber(x);
		}
		inline BOOL WriteUInt32(const UINT32 x)
		{
			return WriteNumber(x);
		}
		inline BOOL WriteUInt64(const UINT64 x)
		{
			return WriteNumber(x);
		}

		inline BOOL WriteString(const TCHAR* x, size_type len)
		{
#ifdef _UNICODE 
			return WriteWChars(x, len);
#else
			return WriteChars(x, len);
#endif
		}


	private: // members
		template <typename T>
		inline BOOL WriteNumber(const T x)
		{
			return WriteBytes(&x, sizeof(T));
		}


	private: // field
		BufferT& m_buffer;
		// typename size_type m_bufferIndex;

	};

	template <typename BufferT>
	BinaryWriter<BufferT>::BinaryWriter(BufferT& buffer)
		: m_buffer(buffer)
		// , m_bufferIndex(0)
	{

	}

	template <typename BufferT>
	BinaryWriter<BufferT>::~BinaryWriter()
	{
	}

	//! \todo 일부 버퍼에서 resize를 아직 구현하지 않아서 주석처리한다.
	template <typename BufferT>
	BOOL BinaryWriter<BufferT>::WriteBytes(const void* buf, size_type len)
	{
		EARLGREY_ASSERT(buf != NULL);
		EARLGREY_ASSERT(len > 0);

		// m_buffer.resize(m_buffer.size() + len);
		m_buffer.append( (BufferT::pointer)buf, len);

		return TRUE;	
	}
}