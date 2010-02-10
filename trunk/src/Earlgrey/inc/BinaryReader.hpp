#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	//! C++은 int, long 등 정수형의 크기와 포맷이 플랫폼이나 컴파일러 구현 등에 맡긴다.
	// 따라서 <inttypes.h> 등을 이용해 플랫폼 간의 상이함을 해결해야 하나 VC++엔 현재 <inttype.h>가 없다.
	template <typename BufferT>
	class BinaryReader : private Uncopyable
	{
	public:
		typedef typename BufferT::size_type size_type;


		explicit BinaryReader(BufferT& buffer);
		~BinaryReader();

		inline size_type Size() const 
		{
			return m_buffer.size();
		}

		inline const BufferT& Buffer() const 
		{
			return m_buffer;
		}
		
		inline BufferT& Buffer()
		{
			return m_buffer;
		}

		inline BOOL ReadBoolean(bool& x)
		{
			return ReadNumber(x);
		}

		inline BOOL ReadBoolean(BOOL& x)
		{
			return ReadNumber(x);
		}

		inline BOOL ReadByte(BYTE& x)
		{
			return ReadBytes(&x, sizeof(x));
		}

		BOOL ReadBytes(void* buf, size_type bufBytes, size_type len);

		inline BOOL ReadChar(CHAR& x)
		{
			return ReadBytes(&x, sizeof(CHAR));
		}

		inline BOOL ReadChars(CHAR* x, size_type x_len, size_type len)
		{
			return ReadBytes(x, x_len * sizeof(CHAR), len * sizeof(CHAR));
		}

		inline BOOL ReadWChar(WCHAR& x)
		{
			return ReadBytes(&x, sizeof(WCHAR));
		}
		
		inline BOOL ReadWChars(WCHAR* x, size_type x_len, size_type len)
		{
			return ReadBytes(x, x_len * sizeof(WCHAR), len * sizeof(WCHAR));
		}


		inline BOOL ReadDouble(double& x)
		{
			return ReadNumber(x);
		}

		inline BOOL ReadFloat(FLOAT& x)
		{
			return ReadNumber(x);
		}

		
		inline BOOL ReadInt8(INT8& x)
		{
			return ReadNumber(x);
		}

		inline BOOL ReadInt16(INT16& x)
		{
			return ReadNumber(x);
		}

		inline BOOL ReadInt32(INT32& x)
		{
			return ReadNumber(x);
		}

		inline BOOL ReadInt64(INT64& x)
		{
			return ReadNumber(x);
		}

		inline BOOL ReadUInt8(UINT8& x)
		{
			return ReadNumber(x);
		}
		
		inline BOOL ReadUInt16(UINT16& x)
		{
			return ReadNumber(x);
		}

		inline BOOL ReadUInt32(UINT32& x)
		{
			return ReadNumber(x);
		}
		
		inline BOOL ReadUInt64(UINT64& x)
		{
			return ReadNumber(x);
		}

		inline BOOL ReadString(WCHAR* x, size_type x_len, size_type len)
		{
			return ReadWChars(x, x_len, len);
		}
		
		inline BOOL ReadString(CHAR* x, size_type x_len, size_type len)
		{
			return ReadChars(x, x_len, len);
		}

	private: // members
		template <typename T>
		inline BOOL ReadNumber(T& x)
		{
			return ReadBytes(&x, sizeof(T), sizeof(T));
		}


	private: // field
		BufferT& m_buffer;
		typename size_type m_bufferIndex;

	};

	template <typename BufferT>
	BinaryReader<BufferT>::BinaryReader(BufferT& buffer)
		: m_buffer(buffer)
		, m_bufferIndex(0)
	{

	}

	template <typename BufferT>
	BinaryReader<BufferT>::~BinaryReader()
	{
	}

	template <typename BufferT>
	BOOL BinaryReader<BufferT>::ReadBytes(void* buf, size_type bufBytes, size_type len)
	{
		EARLGREY_ASSERT(buf != NULL);
		EARLGREY_ASSERT(len > 0);

		size_type lastIndex = m_bufferIndex + len - 1;
		if(lastIndex >= m_buffer.size()) 
		{
			return FALSE;
		}

		memcpy_s(
			buf
			, bufBytes
			, &m_buffer[m_bufferIndex]
			, len 
			);
		m_bufferIndex += len;
		return TRUE;	
	}
}