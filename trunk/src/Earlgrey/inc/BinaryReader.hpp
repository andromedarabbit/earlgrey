#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	//! C++�� int, long �� �������� ũ��� ������ �÷����̳� �����Ϸ� ���� � �ñ��.
	// ���� <inttypes.h> ���� �̿��� �÷��� ���� �������� �ذ��ؾ� �ϳ� VC++�� ���� <inttype.h>�� ����.
	template <typename BufferT>
	class BinaryReader : private Uncopyable
	{
	public:
		typedef typename BufferT::size_type size_type;


		explicit BinaryReader(BufferT& buffer);
		~BinaryReader();

		
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
		BOOL ReadBytes(void* buf, UINT32 len);

		inline BOOL ReadChar(CHAR& x)
		{
			return ReadBytes(&x, sizeof(CHAR));
		}
		inline BOOL ReadChars(CHAR* x, UINT32 len)
		{
			const size_type bytes = len * sizeof(CHAR);
			return ReadBytes(x, bytes);
		}

		inline BOOL ReadWChar(WCHAR& x)
		{
			return ReadBytes(&x, sizeof(WCHAR));
		}
		
		inline BOOL ReadWChars(WCHAR* x, UINT32 len)
		{
			const size_type bytes = len * sizeof(WCHAR);
			return ReadBytes(x, bytes);
		}


		inline BOOL ReadDouble(DOUBLE& x)
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

		inline BOOL ReadString(TCHAR* x, UINT32 len)
		{
#ifdef _UNICODE 
			return ReadWChars(x, len);
#else
			return ReadChars(x, len);
#endif
		}
		

	private: // members
		template <typename T>
		inline BOOL ReadNumber(T& x)
		{
			return ReadBytes(&x, sizeof(T));
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
	BOOL BinaryReader<BufferT>::ReadBytes(void* buf, UINT32 len)
	{
		EARLGREY_ASSERT(buf != NULL);
		EARLGREY_ASSERT(len > 0);

		size_type lastIndex = m_bufferIndex + len - 1;
		if(lastIndex >= m_buffer.size()) 
		{
			return FALSE;
		}

		memcpy(buf, &m_buffer[m_bufferIndex], len);
		m_bufferIndex += len;
		return TRUE;	
	}
}