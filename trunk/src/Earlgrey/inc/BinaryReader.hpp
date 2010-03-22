#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"

#include <type_traits>

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

		BOOL ReadBytes(void* buf, size_type bufBytes, size_type len);

		inline BOOL Read(CHAR* x, size_type x_len, size_type len)
		{
			if(len == 0)
			{
				return TRUE;
			}

			return ReadBytes(x, x_len * sizeof(CHAR), len * sizeof(CHAR));
		}

		inline BOOL Read(WCHAR* x, size_type x_len, size_type len)
		{
			if(len == 0)
			{
				return TRUE;
			}

			return ReadBytes(x, x_len * sizeof(WCHAR), len * sizeof(WCHAR));
		}

		template<class T>
		inline BOOL Read(T& value)
		{
			typedef std::tr1::remove_pointer<T>::type type;
			return Read(value, std::tr1::is_pod<type>(), std::tr1::is_pointer<T>());
		}

		template <typename Key, typename Value>
		inline BOOL Read(std::pair<const Key, Value>& x)
		{
			if(Read(x.first) == FALSE)
				return FALSE;

			if(Read(x.second) == FALSE)
				return FALSE;

			return TRUE;
		}

	private: // members
		// POD 형
		template<class T>
		inline BOOL Read(T& value, std::tr1::true_type, std::tr1::false_type)
		{
			return ReadBytes(&value, sizeof(T), sizeof(T));
		}


		// POD 포인터형
		template<class T>
		BOOL Read(T& value, std::tr1::true_type, std::tr1::true_type)
		{
			typedef std::tr1::remove_pointer<T>::type type;

			// 문자열 포인터는 불러올 수 없으므로 컴파일 에러로 처리한다.
			using std::tr1::is_same;
			using std::tr1::remove_const;
			EARLGREY_STATIC_ASSERT<!is_same<char, remove_const<type>::type>::value>();
// 
// 			bool null;
// 			Copy(&null, sizeof(null));
// 
// 			if (null)
// 			{
// 				value = NULL;
// 				return TRUE;
// 			}
// 
// 			// 메모리를 할당하므로 관리에 주의한다.
// 			value = new type;
// 			Copy(value, sizeof(type));
			return TRUE;
		}


		/*
		// POD 아닌 포인터형
		template<class T>
		void Read(T& value, std::tr1::false_type, std::tr1::true_type)
		{
		using std::tr1::remove_pointer;
		typedef remove_pointer<T>::type type;

		bool null;
		Copy(&null, sizeof(null));

		if (null)
		{
		value = NULL;
		}
		else
		{
		// 메모리를 할당하므로 관리에 주의한다.
		value = new type;
		value->Serialize(*this);
		}
		}
*/
		// POD 아닌 형
		template<class T>
		inline BOOL Read(T& value, std::tr1::false_type, std::tr1::false_type)
		{
			return Serialization::Read(*this, value);
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
	inline BOOL BinaryReader<BufferT>::ReadBytes(void* buf, size_type bufBytes, size_type len)
	{
		if(buf == NULL || len == 0)
			return FALSE;

		size_type lastIndex = m_bufferIndex + len - 1;
		if(lastIndex >= m_buffer.size()) 
		{
			return FALSE;
		}

		memcpy_s(buf, bufBytes, &m_buffer[m_bufferIndex], len);
		m_bufferIndex += len;
		return TRUE;	
	}
}