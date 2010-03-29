#pragma once
#include "BinaryReader.hpp"
#include "txstring.h"

#include <map>
#include <stack>
#include <queue>



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

		inline 
			std::ios::iostate rdstate() const 
		{
			return m_BinaryReader.rdstate();
		}

		inline 
			void clear(std::ios::iostate state = std::ios::goodbit)
		{
			m_BinaryReader.clear(state);
		}

		inline
			void setstate(std::ios::io_state state)
		{
			m_BinaryReader.setstate(state);
		}

		inline
			bool good() const
		{	// test if no state bits are set
			return m_BinaryReader.good();
		}

		inline
			bool eof() const
		{	// test if eofbit is set in stream state
			return m_BinaryReader.eof();
		}

		inline
			bool fail() const
		{	// test if badbit or failbit is set in stream state
			return m_BinaryReader.fail();
		}

		inline
			bool bad() const
		{	// test if badbit is set in stream state
			return m_BinaryReader.bad();
		}


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

		template<typename T>
		inline BOOL Read(T& x)
		{
			return m_BinaryReader.Read(x);
		}

		inline BOOL ReadBytes(void* buf, size_type bufBytes, size_type len)
		{
			return m_BinaryReader.ReadBytes(buf, bufBytes, len);
		}

		inline BOOL ReadString(WCHAR* x, size_type x_len, size_type bufferLen)
		{
			length_type length = 0;
			return ReadString(x, x_len, bufferLen, length);
		}

		inline BOOL ReadString(WCHAR* x, size_type x_len, size_type bufferLen, length_type& length)
		{
			if(m_BinaryReader.Read(length) == FALSE)
				return FALSE;

			if(length == 0)
				return TRUE;

			if(length > bufferLen)
			{
				setstate(std::ios::failbit);
				return FALSE;
			}

			return m_BinaryReader.Read(x, x_len, length);
		}

		NetworkReader& operator>>(xwstring& x);

		inline BOOL ReadString(CHAR* x, size_type x_len, size_type bufferLen)
		{
			length_type length = 0;
			return ReadString(x, x_len, bufferLen, length);
		}

		inline BOOL ReadString(CHAR* x, size_type x_len, size_type bufferLen, length_type& length)
		{
			if(m_BinaryReader.Read(length) == FALSE)
				return FALSE;

			if(length > bufferLen)
			{
				setstate(std::ios::failbit);
				return FALSE;
			}

			return m_BinaryReader.Read(x, x_len, length);
		}

		NetworkReader& operator >> (xstring& x);

		template<class Kty, class Ty, class Pr, class Alloc>
		NetworkReader& operator >> (std::map<Kty, Ty, Pr, Alloc>& container);

		template<typename T, typename Alloc>
		NetworkReader& operator >> (std::vector<T, Alloc>& container);

		template<typename T, typename C>
		NetworkReader& operator >> (std::stack<T, C>& container);

		template<class Kty, class Pr, class Alloc>
		NetworkReader& operator >> (std::set<Kty, Pr, Alloc>& container);

		template<typename T, typename C>
		NetworkReader& operator >> (std::queue<T, C>& container);

		template<typename T>
		NetworkReader& operator >> (std::deque<T>& container);

		template<typename T>
		inline NetworkReader& operator>>(T& x)
		{
			this->Read(x); // return 값 확인 안 해도 Read 안에서 오류 값 설정함
			return *this;
		}

	private: // methods
		template<class T>
		inline BOOL ReadContainer(T& container)
		{
			length_type size = 0;
			if(m_BinaryReader.Read(size) == FALSE)
				return FALSE;

			for(length_type i = 0; i < size; i++)
			{
				T::value_type v;
				if(m_BinaryReader.Read(v) == FALSE)
					return FALSE;

				std::back_inserter(container) = v;
			}

			return TRUE;
		}

	private: // field
		BinaryReader<BufferT> m_BinaryReader;
		std::ios::iostate m_State;

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
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(xwstring& x)
	{
		static __declspec(thread) TCHAR buffer[_UI16_MAX];
		buffer[0] = NULL;

		length_type length = 0;
		if(this->ReadString(buffer, _countof(buffer), _UI16_MAX, length) == FALSE)
		{
			// TODO
			throw std::exception("");
		}

		buffer[length] = NULL;
		if(length == 0)
			return *this;

		x.assign(buffer, length);
		return *this;
	}

	template <typename BufferT>
	inline NetworkReader<BufferT>& NetworkReader<BufferT>::operator>>(xstring& x)
	{
		static __declspec(thread) CHAR buffer[_UI16_MAX];
		buffer[0] = NULL;

		length_type length = 0;
		if(this->ReadString(buffer, _countof(buffer), _UI16_MAX, length) == FALSE)
		{
			// TODO
			throw std::exception("");
		}

		buffer[length] = NULL;
		if(length == 0)
			return *this;

		x.assign(buffer, length);
		return *this;
	}

	template <typename BufferT>
	template<class Kty, class Ty, class Pr, class Alloc>
	inline NetworkReader<BufferT>& 
		NetworkReader<BufferT>::operator >> (std::map<Kty, Ty, Pr, Alloc>& container)
	{
		if(ReadContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<typename T, typename Alloc>
	inline NetworkReader<BufferT>& 
		NetworkReader<BufferT>::operator >> (std::vector<T, Alloc>& container)
	{
		if(ReadContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<typename T, typename C>
	inline NetworkReader<BufferT>& 
		NetworkReader<BufferT>::operator >> (std::stack<T, C>& container)
	{
		if(ReadContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<class Kty, class Pr, class Alloc>
	inline NetworkReader<BufferT>& 
		NetworkReader<BufferT>::operator >> (std::set<Kty, Pr, Alloc>& container)
	{
		if(ReadContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<typename T, typename C>
	inline NetworkReader<BufferT>&
		NetworkReader<BufferT>::operator >> (std::queue<T, C>& container)
	{
		if(ReadContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<typename T>
	inline NetworkReader<BufferT>& 
		NetworkReader<BufferT>::operator >> (std::deque<T>& container)
	{
		if(ReadContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}
}