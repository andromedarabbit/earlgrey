#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"

#include <type_traits>

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
		// POD ��
		template<class T>
		inline BOOL Read(T& value, std::tr1::true_type, std::tr1::false_type)
		{
			return ReadBytes(&value, sizeof(T), sizeof(T));
		}


		// POD ��������
// 		template<class T>
// 		BOOL Read(T& value, std::tr1::true_type, std::tr1::true_type)
// 		{
// 			typedef std::tr1::remove_pointer<T>::type type;
// 
// 			// ���ڿ� �����ʹ� �ҷ��� �� �����Ƿ� ������ ������ ó���Ѵ�.
// 			using std::tr1::is_same;
// 			using std::tr1::remove_const;
// 			EARLGREY_STATIC_ASSERT<!is_same<char, remove_const<type>::type>::value>();
// // 
// // 			bool null;
// // 			Copy(&null, sizeof(null));
// // 
// // 			if (null)
// // 			{
// // 				value = NULL;
// // 				return TRUE;
// // 			}
// // 
// // 			// �޸𸮸� �Ҵ��ϹǷ� ������ �����Ѵ�.
// // 			value = new type;
// // 			Copy(value, sizeof(type));
// 			return TRUE;
// 		}


		/*
		// POD �ƴ� ��������
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
				// �޸𸮸� �Ҵ��ϹǷ� ������ �����Ѵ�.
				value = new type;
				value->Serialize(*this);
				}
			}
*/
		// POD �ƴ� ��
		template<class T>
		inline BOOL Read(T& value, std::tr1::false_type, std::tr1::false_type)
		{
			if(good() == false) // badbit, failbit, eofbit
				return FALSE;

			if(Serialization::Read(*this, value) == FALSE)
			{
				setstate(std::ios::failbit);
				return FALSE;
			}
			return TRUE;
		}



	private: // field
		BufferT& m_buffer;
		typename size_type m_bufferIndex;
		std::ios::iostate m_State;
	};

	template <typename BufferT>
	BinaryReader<BufferT>::BinaryReader(BufferT& buffer)
		: m_buffer(buffer)
		, m_bufferIndex(0)
		, m_State(std::ios::goodbit)
	{

	}

	template <typename BufferT>
	BinaryReader<BufferT>::~BinaryReader()
	{
	}

	template <typename BufferT>
	inline BOOL BinaryReader<BufferT>::ReadBytes(void* buf, size_type bufBytes, size_type len)
	{
		if(good() == FALSE)
			return FALSE;

		if(buf == NULL || len == 0)
			return FALSE;

		size_type lastIndex = m_bufferIndex + len - 1;
		if(lastIndex >= m_buffer.size()) 
		{
			setstate(std::ios::badbit);
			return FALSE;
		}

		// TODO: eof�� �ǰ� ���� Writer�� �����͸� ���ۿ� �Է��ϸ� ���Ѵ�.
// 		if((lastIndex + 1) == m_buffer.size()) 
// 			setstate(std::ios::eofbit);

		memcpy_s(buf, bufBytes, &m_buffer[m_bufferIndex], len);
		m_bufferIndex += len;
		return TRUE;	
	}
}