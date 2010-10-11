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

		inline 
			std::ios::iostate rdstate() const 
		{
			return m_State;
		}

		inline 
			void clear(std::ios::iostate state = std::ios::goodbit)
		{
			m_State = state;
		}

		inline
			void setstate(std::ios::io_state state)
		{
			clear ( rdstate() | state );
		}

		inline
			bool good() const
		{	// test if no state bits are set
			return (rdstate() == std::ios::goodbit);
		}

		inline
			bool eof() const
		{	// test if eofbit is set in stream state
			return ((int)rdstate() & (int)std::ios::eofbit);
		}

		inline
			bool fail() const
		{	// test if badbit or failbit is set in stream state
			return (((int)rdstate()
				& ((int)std::ios::badbit | (int)std::ios::failbit)) != 0);
		}

		inline
			bool bad() const
		{	// test if badbit is set in stream state
			return (((int)rdstate() & (int)std::ios::badbit) != 0);
		}


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
			typedef std::tr1::remove_pointer<T>::type type;

			const size_type bytes = len * sizeof(type);
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
// 		template<class T>
// 		inline BOOL Write(const T& value, std::tr1::true_type, std::tr1::true_type, size_type len)
// 		{
// 			if(good() == false) // badbit, failbit, eofbit
// 				return FALSE;

// 			const size_type bytes = len * sizeof(T);
// 			if(WriteBytes(&value, bytes) == FALSE)
// 			{
// 				setstate(std::ios::failbit);
// 				return FALSE;
// 			}
// 			return TRUE;
// 		}

		/*
		// POD 아닌 포인터형
		template<class T>
		inline BOOL Write(const T& value, std::tr1::false_type, std::tr1::true_type, size_type len)
		{
			if(Serializer<T>::Write(*this, value, n) == FALSE)
			{
				setstate(std::ios::failbit);
				return FALSE;
			}
			return TRUE;
		}
*/
		// POD 아닌 형
		template<class T>
		inline BOOL Write(const T& value, std::tr1::false_type, std::tr1::false_type)
		{
			if(good() == false) // badbit, failbit, eofbit
				return FALSE;

			if(Serialization::Write(*this, value) == FALSE)
			{
				setstate(std::ios::failbit);
				return FALSE;
			}
			return TRUE;
		}


	private: // field
		BufferT& m_buffer;
		std::ios::iostate m_State;

	};

	template <typename BufferT>
	BinaryWriter<BufferT>::BinaryWriter(BufferT& buffer)
		: m_buffer(buffer)
		, m_State(std::ios::goodbit)
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

		if(good() == false) // badbit, failbit, eofbit
			return FALSE;

		if(len == 0)
			return TRUE;

		// m_buffer.resize(m_buffer.size() + len);
		try
		{
			m_buffer.append( (BufferT::pointer)buf, len);
		}
		catch(std::out_of_range&)
		{
			setstate(std::ios::badbit); // TODO: eofbit 도 넣어야 하나?
			return FALSE;
		}		

		return TRUE;	
	}
}