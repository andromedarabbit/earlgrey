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

			if(length > bufferLen)
				return FALSE;

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
				return FALSE;

			return m_BinaryReader.Read(x, x_len, length);
		}

		NetworkReader& operator>>(xstring& x);



		template<class Kty, class Ty, class Pr, class Alloc>
		NetworkReader& operator >>(std::map<Kty, Ty, Pr, Alloc>& container);

		template<typename T, typename Alloc>
		NetworkReader& operator >>(std::vector<T, Alloc>& container);

		template<typename T, typename C>
		NetworkReader& operator >>(std::stack<T, C>& container);

		template<class Kty, class Pr, class Alloc>
		NetworkReader& operator >>(std::set<Kty, Pr, Alloc>& container);

		template<typename T, typename C>
		NetworkReader& operator >>(std::queue<T, C>& container);

		template<typename T>
		NetworkReader& operator >>(std::deque<T>& container);

		template<typename T>
		inline NetworkReader& operator>>(T& x)
		{
			if(this->Read(x) == FALSE)
			{
				// TODO
				throw std::exception("");
			}
			return *this;
		}

	private: // methods
		template<class Kty, class Ty, class Pr, class Alloc>
		inline BOOL ReadContainer(std::map<Kty, Ty, Pr, Alloc>& container)
		{
			length_type size = 0;
			if(m_BinaryReader.Read(size) == FALSE)
				return FALSE;

			for(length_type i = 0; i < size; i++)
			{
				std::pair<const Kty, Ty> v;
				if(m_BinaryReader.Read(v) == FALSE)
					return FALSE;

				container.insert(v);
			}

			return TRUE;
		}

		template<typename T, typename Alloc>
		inline BOOL ReadContainer(std::vector<T, Alloc>& container)
		{
			length_type size = 0;
			if(m_BinaryReader.Read(size) == FALSE)
				return FALSE;

			for(length_type i = 0; i < size; i++)
			{
				T v;
				if(m_BinaryReader.Read(v) == FALSE)
					return FALSE;

				container.push_back(v);
			}

			return TRUE;
		}

		template<typename T, typename C>
		inline BOOL ReadContainer(std::stack<T, C>& container)
		{
			length_type size = 0;
			if(m_BinaryReader.Read(size) == FALSE)
				return FALSE;

			for(length_type i = 0; i < size; i++)
			{
				T v;
				if(m_BinaryReader.Read(v) == FALSE)
					return FALSE;

				container.push(v);
			}

			return TRUE;
		}

		template<class Kty, class Pr, class Alloc>
		inline BOOL ReadContainer(std::set<Kty, Pr, Alloc>& container)
		{
			length_type size = 0;
			if(m_BinaryReader.Read(size) == FALSE)
				return FALSE;

			for(length_type i = 0; i < size; i++)
			{
				Key v;
				if(m_BinaryReader.Read(v) == FALSE)
					return FALSE;

				container.insert(v);
			}

			return TRUE;
		}

		template<typename T, typename C>
		inline BOOL ReadContainer(std::queue<T, C>& container)
		{
			length_type size = 0;
			if(m_BinaryReader.Read(size) == FALSE)
				return FALSE;

			for(length_type i = 0; i < size; i++)
			{
				T v;
				if(m_BinaryReader.Read(v) == FALSE)
					return FALSE;

				container.push(v);
			}

			return TRUE;
		}

		template<typename T>
		inline BOOL ReadContainer(std::deque<T>& container)
		{
			length_type size = 0;
			if(m_BinaryReader.Read(size) == FALSE)
				return FALSE;

			for(length_type i = 0; i < size; i++)
			{
				T v;
				if(m_BinaryReader.Read(v) == FALSE)
					return FALSE;

				container.push_back(v);
			}

			return TRUE;
		}

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
		x.assign(buffer, length);
		return *this;
	}

	template <typename BufferT>
	template<class Kty, class Ty, class Pr, class Alloc>
	inline NetworkReader<BufferT>& 
		NetworkReader<BufferT>::operator >>(std::map<Kty, Ty, Pr, Alloc>& container)
	{
		if(ReadContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<typename T, typename Alloc>
	inline NetworkReader<BufferT>& 
		NetworkReader<BufferT>::operator >>(std::vector<T, Alloc>& container)
	{
		if(ReadContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<typename T, typename C>
	inline NetworkReader<BufferT>& 
		NetworkReader<BufferT>::operator >>(std::stack<T, C>& container)
	{
		if(ReadContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<class Kty, class Pr, class Alloc>
	inline NetworkReader<BufferT>& 
		NetworkReader<BufferT>::operator >>(std::set<Kty, Pr, Alloc>& container)
	{
		if(ReadContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<typename T, typename C>
	inline NetworkReader<BufferT>&
		NetworkReader<BufferT>::operator >>(std::queue<T, C>& container)
	{
		if(ReadContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<typename T>
	inline NetworkReader<BufferT>& 
		NetworkReader<BufferT>::operator >>(std::deque<T>& container)
	{
		if(ReadContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}
}