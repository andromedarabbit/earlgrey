#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"

#include <type_traits>

namespace Earlgrey
{
	//! C++은 int, long 등 정수형의 크기와 포맷이 플랫폼이나 컴파일러 구현 등에 맡긴다.
	// 따라서 <inttypes.h> 등을 이용해 플랫폼 간의 상이함을 해결해야 하나 VC++엔 현재 <inttype.h>가 없다.

	//! BinaryWriter는 기본적으로 POD 타입만 지원한다.
	template <typename BufferT>
	class BinaryWriter : private Uncopyable
	{
	public:
		typedef typename BufferT::size_type size_type;


		explicit BinaryWriter(BufferT& buffer);
		~BinaryWriter();

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


		BOOL WriteBytes(const void* buf, size_type len);


		template<typename T>
		inline BOOL Write(const T x, size_type len)
		{
			const size_type bytes = len * sizeof(T);
			return WriteBytes(x, bytes);
			// typedef std::tr1::remove_pointer<T>::type type;
			// return Write(value, std::tr1::is_pod<type>(), std::tr1::is_pointer<T>());
		}

		template <typename Key, typename Value>
		inline BOOL Write(const std::pair<const Key, Value>& x)
		{
			if(Write(x.first) == FALSE)
				return FALSE;

			if(Write(x.second) == FALSE)
				return FALSE;

			return TRUE;
		}

		template<class T>
		inline BOOL Write(const T& value)
		{
			typedef std::tr1::remove_pointer<T>::type type;
			return Write(value, std::tr1::is_pod<type>(), std::tr1::is_pointer<T>());
		}

		/*
		template<class T>
		inline BOOL Write(const T& value, size_type len)
		{
			typedef std::tr1::remove_pointer<T>::type tt;
			return Write(value, std::tr1::is_pod<tt>(), std::tr1::is_pointer<T>(), len);
		}
		*/

		inline BOOL Write(const WCHAR* x)
		{
			return Write(x, wcslen(x));
		}

		inline BOOL Write(const CHAR* x)
		{
			return Write(x, strlen(x));
		}

	private: // members
		// POD 이면서 포인터형도 아님
		template<class T>
		inline BOOL Write(const T& value, std::tr1::true_type, std::tr1::false_type)
		{
			return WriteBytes(&value, sizeof(value));
		}

		// POD 포인터형
		template<class T>
		inline BOOL Write(const T& value, std::tr1::true_type, std::tr1::true_type, size_type len)
		{
			const size_type bytes = len * sizeof(T);
			return WriteBytes(&value, bytes);
		}

		/*
		// POD 아닌 포인터형
		template<class T>
		inline BOOL Write(const T& value, std::tr1::false_type, std::tr1::true_type, size_type len)
		{
			return Serializer<T>::Write(*this, value, n);
		}

		// POD 아닌 형
		template<class T>
		BOOL Write(const T& value, std::tr1::false_type, std::tr1::false_type)
		{
			return Serializer<T>::Write(*this, &value);
		}
		*/

	private: // field
		BufferT& m_buffer;

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
		// EARLGREY_ASSERT(buf != NULL);
		// EARLGREY_ASSERT(len > 0);
		EARLGREY_ASSERT( buf != NULL || len == 0 );

		if(len == 0)
			return TRUE;

		// m_buffer.resize(m_buffer.size() + len);
		m_buffer.append( (BufferT::pointer)buf, len);

		return TRUE;	
	}
}