#pragma once
#include "BinaryReader.hpp"
#include "txstring.h"

namespace Earlgrey
{
	template <typename BufferT>
	class NetworkReader : private Uncopyable
	{
	public:
		typedef typename BinaryReader<BufferT>::size_type size_type;
		typedef UINT16 length_type;

		explicit NetworkReader(BufferT& buffer);
		~NetworkReader();

		inline size_type Size() const 
		{
			return m_BinaryReader.Size();
		}

		inline const BufferT& Buffer() const 
		{
			return m_BinaryReader.Buffer();
		}

		inline BufferT& Buffer()
		{
			return m_BinaryReader.Buffer();
		}

		inline BOOL ReadBoolean(bool& x)
		{
			return m_BinaryReader.ReadBoolean(x);
		}

		NetworkReader& operator>>(bool& x);

		inline BOOL ReadBoolean(BOOL& x)
		{
			return m_BinaryReader.ReadBoolean(x);
		}

		inline BOOL ReadByte(BYTE& x)
		{
			return m_BinaryReader.ReadByte(&x, sizeof(x));
		}
		
		inline BOOL ReadBytes(void* buf, size_type len)
		{
			return m_BinaryReader.ReadBytes(buf, len);
		}

		inline BOOL ReadChar(CHAR& x)
		{
			return m_BinaryReader.ReadChar(&x, sizeof(CHAR));
		}

		NetworkReader& operator>>(CHAR& x);

		inline BOOL ReadChars(CHAR* x, size_type len)
		{
			return m_BinaryReader.ReadChars(x, len);
		}

		inline BOOL ReadWChar(WCHAR& x)
		{
			return m_BinaryReader.ReadWChar(x);
		}

		NetworkReader& operator>>(WCHAR& x);

		inline BOOL ReadWChars(WCHAR* x, size_type len)
		{
			return m_BinaryReader.ReadWChars(x, len);
		}

		inline BOOL ReadDouble(double& x)
		{
			return m_BinaryReader.ReadDouble(x);
		}

		NetworkReader& operator>>(double& x);

		inline BOOL ReadFloat(FLOAT& x)
		{
			return m_BinaryReader.ReadFloat(x);
		}

		NetworkReader& operator>>(FLOAT& x);


		inline BOOL ReadInt8(INT8& x)
		{
			return m_BinaryReader.ReadInt8(x);
		}

		NetworkReader& operator>>(INT8& x);

		inline BOOL ReadInt16(INT16& x)
		{
			return m_BinaryReader.ReadInt16(x);
		}

		NetworkReader& operator>>(INT16& x);

		inline BOOL ReadInt32(INT32& x)
		{
			return m_BinaryReader.ReadInt32(x);
		}

		NetworkReader& operator>>(INT32& x);

		inline BOOL ReadInt64(INT64& x)
		{
			return m_BinaryReader.ReadInt64(x);
		}

		NetworkReader& operator>>(INT64& x);

		inline BOOL ReadUInt8(UINT8& x)
		{
			return m_BinaryReader.ReadUInt8(x);
		}

		NetworkReader& operator>>(UINT8& x);

		inline BOOL ReadUInt16(UINT16& x)
		{
			return m_BinaryReader.ReadUInt16(x);
		}

		NetworkReader& operator>>(UINT16& x);

		inline BOOL ReadUInt32(UINT32& x)
		{
			return m_BinaryReader.ReadUInt32(x);
		}
		
		NetworkReader& operator>>(UINT32& x);

		inline BOOL ReadUInt64(UINT64& x)
		{
			return m_BinaryReader.ReadUInt64(x);
		}

		NetworkReader& operator>>(UINT64& x);

		inline BOOL ReadString(WCHAR* x, size_type bufferLen)
		{
			length_type length = 0;

			if(m_BinaryReader.ReadUInt16(length) == FALSE)
				return FALSE;

			if(length > bufferLen)
				return FALSE;

			return m_BinaryReader.ReadString(x, length);
		}

		NetworkReader& operator>>(xwstring& x);

		inline BOOL ReadString(CHAR* x, size_type bufferLen)
		{
			length_type length = 0;

			if(m_BinaryReader.ReadUInt16(length) == FALSE)
				return FALSE;

			if(length > bufferLen)
				return FALSE;

			return m_BinaryReader.ReadString(x, length);
		}

		NetworkReader& operator>>(xstring& x);

	private: // field
		BinaryReader<BufferT> m_BinaryReader;

	};

	template <typename BufferT>
	NetworkReader<BufferT>::NetworkReader(BufferT& buffer)
		: m_BinaryReader(buffer)
	{

	}

	template <typename BufferT>
	NetworkReader<BufferT>::~NetworkReader()
	{
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(bool& x)
	{
		if(this->ReadBoolean(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	//template <typename BufferT>
	//inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(BOOL x)
	//{
	//	if(this->ReadBoolean(x) == FALSE)
	//	{
	//		// TODO
	//		throw std::exception("");
	//	}
	//	return *this;
	//}

	//template <typename BufferT>
	//inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(BYTE x)
	//{
	//	if(this->ReadByte(x) == FALSE)
	//	{
	//		// TODO
	//		throw std::exception("");
	//	}
	//	return *this;
	//}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(CHAR& x)
	{
		if(this->ReadChar(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(WCHAR& x)
	{
		if(this->ReadWChar(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(double& x)
	{
		if(this->ReadDouble(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(FLOAT& x)
	{
		if(this->ReadFloat(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(INT8& x)
	{
		if(this->ReadInt8(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(INT16& x)
	{
		if(this->ReadInt16(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(INT32& x)
	{
		if(this->ReadInt32(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(INT64& x)
	{
		if(this->ReadInt64(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}


	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(UINT8& x)
	{
		if(this->ReadUInt8(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(UINT16& x)
	{
		if(this->ReadUInt16(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(UINT32& x)
	{
		if(this->ReadUInt32(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(UINT64& x)
	{
		if(this->ReadUInt64(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(xwstring& x)
	{
		static __declspec(thread) TCHAR buffer[_UI16_MAX];
		if(this->ReadString(buffer, _UI16_MAX) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		x = buffer;
		return *this;
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(xstring& x)
	{
		static __declspec(thread) CHAR buffer[_UI16_MAX];
		if(this->ReadString(buffer, _UI16_MAX) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		x = buffer;
		return *this;
	}
}