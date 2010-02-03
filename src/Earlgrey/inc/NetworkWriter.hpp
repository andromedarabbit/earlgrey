#pragma once
#include "BinaryWriter.hpp"

namespace Earlgrey
{
	//! C++은 int, long 등 정수형의 크기와 포맷이 플랫폼이나 컴파일러 구현 등에 맡긴다.
	// 따라서 <inttypes.h> 등을 이용해 플랫폼 간의 상이함을 해결해야 하나 VC++엔 현재 <inttype.h>가 없다.
	template <typename BufferT>
	class NetworkWriter : private Uncopyable
	{
	public:
		typedef typename BinaryWriter<BufferT>::size_type size_type;
		typedef UINT16 length_type;

		explicit NetworkWriter(BufferT& buffer);
		~NetworkWriter();

		inline size_type Size() const 
		{
			return m_BinaryWriter.Size();
		}

		inline const BufferT& Buffer() const 
		{
			return m_BinaryWriter.Buffer();
		}

		inline BufferT& Buffer()
		{
			return m_BinaryWriter.Buffer();
		}

		inline BOOL WriteBoolean(const bool x)
		{
			return m_BinaryWriter.WriteBoolean(x);
		}

		NetworkWriter& operator<<(bool x);
		
		inline BOOL WriteBoolean(const BOOL x)
		{
			return m_BinaryWriter.WriteBoolean(x);
		}

		// NetworkWriter& operator<<(BOOL x); // BOOL == int == INT32

		inline BOOL WriteByte(const BYTE x)
		{
			return m_BinaryWriter.WriteByte(x);
		}

		// NetworkWriter& operator<<(BYTE x); // BYTE == unsigned char == INT8

		BOOL WriteBytes(const void* buf, size_type len)
		{
			return m_BinaryWriter.WriteBytes(buf, len);
		}

		inline BOOL WriteChar(const CHAR x)
		{
			return m_BinaryWriter.WriteChar(x);
		}

		inline NetworkWriter& operator<<(CHAR x);

		inline BOOL WriteChars(const CHAR* x, size_type len)
		{
			return m_BinaryWriter.WriteChars(x, len);
		}

		inline BOOL WriteWChar(const WCHAR x)
		{
			return m_BinaryWriter.WriteWChar(x);
		}

		NetworkWriter& operator<<(WCHAR x);

		inline BOOL WriteWChars(const WCHAR* x, size_type len)
		{
			return m_BinaryWriter.WriteWChars(x, len);
		}


		inline BOOL WriteDouble(const double x)
		{
			return m_BinaryWriter.WriteDouble(x);
		}

		NetworkWriter& operator<<(double x);

		inline BOOL WriteFloat(const FLOAT x)
		{
			return m_BinaryWriter.WriteFloat(x);
		}

		inline NetworkWriter& operator<<(FLOAT x);


		inline BOOL WriteInt8(const INT8 x)
		{
			return m_BinaryWriter.WriteInt8(x);
		}

		NetworkWriter& operator<<(INT8 x);

		inline BOOL WriteInt16(const INT16 x)
		{
			return m_BinaryWriter.WriteInt16(x);
		}

		NetworkWriter& operator<<(INT16 x);

		inline BOOL WriteInt32(const INT32 x)
		{
			return m_BinaryWriter.WriteInt32(x);
		}

		NetworkWriter& operator<<(INT32 x);

		inline BOOL WriteInt64(const INT64 x)
		{
			return m_BinaryWriter.WriteInt64(x);
		}

		NetworkWriter& operator<<(INT64 x);

		inline BOOL WriteUInt8(const UINT8 x)
		{
			return m_BinaryWriter.WriteUInt8(x);
		}

		NetworkWriter& operator<<(UINT8 x);

		inline BOOL WriteUInt16(const UINT16 x)
		{
			return m_BinaryWriter.WriteUInt16(x);
		}

		NetworkWriter& operator<<(UINT16 x);

		inline BOOL WriteUInt32(const UINT32 x)
		{
			return m_BinaryWriter.WriteUInt32(x);
		}

		NetworkWriter& operator<<(UINT32 x);

		inline BOOL WriteUInt64(const UINT64 x)
		{
			return m_BinaryWriter.WriteUInt64(x);
		}

		NetworkWriter& operator<<(UINT64 x);

		inline BOOL WriteString(const WCHAR* x)
		{
			// TODO: numeric_cast
			length_type len = static_cast<length_type>(wcslen(x));
			return WriteString(x, len);
		}

		NetworkWriter& operator<<(const WCHAR* x);
		NetworkWriter& operator<<(const xwstring& x);

		inline BOOL WriteString(const WCHAR* x, length_type length)
		{
			if(m_BinaryWriter.WriteUInt16(length) == FALSE)
				return FALSE;

			return m_BinaryWriter.WriteString(x, length);
		}

		inline BOOL WriteString(const CHAR* x)
		{
			return WriteString(x, strlen(x));
		}

		NetworkWriter& operator<<(const CHAR* x);
		NetworkWriter& operator<<(const xstring& x);

		inline BOOL WriteString(const CHAR* x, length_type length)
		{
			if(m_BinaryWriter.WriteUInt16(length) == FALSE)
				return FALSE;

			return m_BinaryWriter.WriteString(x, length);
		}




	private: // field
		BinaryWriter<BufferT> m_BinaryWriter;
		
	};

	template <typename BufferT>
	NetworkWriter<BufferT>::NetworkWriter(BufferT& buffer)
		: m_BinaryWriter(buffer)
	{

	}

	template <typename BufferT>
	NetworkWriter<BufferT>::~NetworkWriter()
	{
	}


	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(bool x)
	{
		if(this->WriteBoolean(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	//template <typename BufferT>
	//inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(BOOL x)
	//{
	//	if(this->WriteBoolean(x) == FALSE)
	//	{
	//		// TODO
	//		throw std::exception("");
	//	}
	//	return *this;
	//}

	//template <typename BufferT>
	//inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(BYTE x)
	//{
	//	if(this->WriteByte(x) == FALSE)
	//	{
	//		// TODO
	//		throw std::exception("");
	//	}
	//	return *this;
	//}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(CHAR x)
	{
		if(this->WriteChar(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(WCHAR x)
	{
		if(this->WriteWChar(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(double x)
	{
		if(this->WriteDouble(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(FLOAT x)
	{
		if(this->WriteFloat(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(INT8 x)
	{
		if(this->WriteInt8(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(INT16 x)
	{
		if(this->WriteInt16(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(INT32 x)
	{
		if(this->WriteInt32(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(INT64 x)
	{
		if(this->WriteInt64(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}


	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(UINT8 x)
	{
		if(this->WriteUInt8(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(UINT16 x)
	{
		if(this->WriteUInt16(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(UINT32 x)
	{
		if(this->WriteUInt32(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(UINT64 x)
	{
		if(this->WriteUInt64(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(const WCHAR * x)
	{
		if(this->WriteString(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(const xwstring& x)
	{
		if(this->WriteString(x, x.length()) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(const CHAR * x)
	{
		if(this->WriteString(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(const xstring& x)
	{
		if(this->WriteString(x, x.length()) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}
}