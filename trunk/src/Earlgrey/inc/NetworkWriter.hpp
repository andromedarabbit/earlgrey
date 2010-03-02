#pragma once
#include "BinaryWriter.hpp"
#include "numeric_cast.hpp"

#include <map>
#include <stack>
#include <queue>

namespace Earlgrey
{
	//! C++�� int, long �� �������� ũ��� ������ �÷����̳� �����Ϸ� ���� � �ñ��.
	// ���� <inttypes.h> ���� �̿��� �÷��� ���� �������� �ذ��ؾ� �ϳ� VC++�� ���� <inttype.h>�� ����.
	template <typename BufferT>
	class NetworkWriter : private Uncopyable
	{
	public:
		typedef BinaryWriter<BufferT> RawWriter;
		typedef typename RawWriter::size_type size_type;
		typedef UINT16 length_type;

		explicit NetworkWriter(BufferT& buffer);
		~NetworkWriter();

		size_type Size() const;

		const BufferT& Buffer() const;

		BufferT& Buffer();

		BOOL WriteBytes(const void* buf, size_type len);
	

		template <typename T>
		inline NetworkWriter& operator<<(const T& x)
		{
			if(this->Write(x) == FALSE)
			{
				// TODO
				throw std::exception("");
			}
			return *this;
		}

		inline BOOL Write(const WCHAR* x)
		{
			// TODO: numeric_cast
			length_type len = static_cast<length_type>(wcslen(x));
			return Write(x, len);
		}

		inline BOOL Write(const WCHAR* x, length_type length)
		{
			if(m_BinaryWriter.Write(length) == FALSE)
				return FALSE;

			return m_BinaryWriter.Write(x, length);
		}

		template <typename T>
		inline BOOL Write(const T& x)
		{
			return m_BinaryWriter.Write(x);
		}


		NetworkWriter& operator<<(const WCHAR* x);
		NetworkWriter& operator<<(const xwstring& x);


		inline BOOL WriteString(const CHAR* x)
		{
			return WriteString(x, EARLGREY_NUMERIC_CAST<length_type>(strlen(x)));
		}

		inline BOOL WriteString(const CHAR* x, length_type length)
		{
			if(m_BinaryWriter.Write(length) == FALSE)
				return FALSE;

			return m_BinaryWriter.Write(x, length);
		}

		NetworkWriter& operator<<(const CHAR* x);
		NetworkWriter& operator<<(const xstring& x);
	

		template<class Kty, class Ty, class Pr, class Alloc>
			NetworkWriter& operator <<(const std::map<Kty, Ty, Pr, Alloc>& container);

		template<typename T, typename Alloc>
			NetworkWriter& operator <<(const std::vector<T, Alloc>& container);

		template<typename T, typename C>
			NetworkWriter& operator <<(const std::stack<T, C>& container);

		template<class Kty, class Pr, class Alloc>
			NetworkWriter& operator <<(const std::set<Kty, Pr, Alloc>& container);

		template<typename T, typename C>
			NetworkWriter& operator <<(const std::queue<T, C>& container);

		template<typename T>
			NetworkWriter& operator <<(const std::deque<T>& container);

	private: // methods
		/*
		template<class Kty, class Ty, class Pr, class Alloc>
			inline BOOL WriteContainer(const std::map<Kty, Ty, Pr, Alloc>& container)
		{
			typedef std::map<Kty, Ty, Pr, Alloc> map_type;

			if(m_BinaryWriter.Write(container.size()) == FALSE)
				return FALSE;

			typedef typename map_type::const_iterator const_iterator;
			for (const_iterator it = container.begin(); it != container.end(); ++it)
			{
				if(m_BinaryWriter.Write(it->first) == FALSE)
					return FALSE;
				
				if(m_BinaryWriter.Write(it->second) == FALSE)
					return FALSE;
			}

			return TRUE;
		}
			*/

		template<class Container>
			inline BOOL WriteContainer(const Container& container)
		{
			const length_type size = EARLGREY_NUMERIC_CAST<length_type>(container.size());
			if(m_BinaryWriter.Write(size) == FALSE)
				return FALSE;

			Container::const_iterator it = container.begin();
			for( ; it != container.end(); it++)
			{
				if(m_BinaryWriter.Write(*it) == FALSE)
					return FALSE;
			}
			return TRUE;
		}

	private: // field
		RawWriter m_BinaryWriter;

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
	inline typename NetworkWriter<BufferT>::size_type NetworkWriter<BufferT>::Size() const 
	{
		return m_BinaryWriter.Size();
	}

	template <typename BufferT>
	inline const BufferT& NetworkWriter<BufferT>::Buffer() const 
	{
		return m_BinaryWriter.Buffer();
	}

	template <typename BufferT>
	inline BufferT& NetworkWriter<BufferT>::Buffer()
	{
		return m_BinaryWriter.Buffer();
	}

	template <typename BufferT>
	inline BOOL NetworkWriter<BufferT>::WriteBytes(const void* buf, size_type len)
	{
		return m_BinaryWriter.WriteBytes(buf, len);
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(const WCHAR * x)
	{
		if(this->Write(x) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(const xwstring& x)
	{
		if(this->Write(x.c_str(), EARLGREY_NUMERIC_CAST<length_type>(x.length())) == FALSE)
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
		if(this->WriteString(x.c_str(), EARLGREY_NUMERIC_CAST<length_type>(x.length())) == FALSE)
		{
			// TODO
			throw std::exception("");
		}
		return *this;
	}

	template <typename BufferT>
	template<class Kty, class Ty, class Pr, class Alloc>
	inline NetworkWriter<BufferT>& 
		NetworkWriter<BufferT>::operator <<(const std::map<Kty, Ty, Pr, Alloc>& container)
	{
		if(WriteContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<typename T, typename Alloc>
	inline NetworkWriter<BufferT>& 
		NetworkWriter<BufferT>::operator <<(const std::vector<T, Alloc>& container)
	{
		if(WriteContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<typename T, typename C>
	inline NetworkWriter<BufferT>& 
		NetworkWriter<BufferT>::operator <<(const std::stack<T, C>& container)
	{
		if(WriteContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<class Kty, class Pr, class Alloc>
	inline NetworkWriter<BufferT>& 
		NetworkWriter<BufferT>::operator <<(const std::set<Kty, Pr, Alloc>& container)
	{
		if(WriteContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<typename T, typename C>
	inline NetworkWriter<BufferT>&
		NetworkWriter<BufferT>::operator <<(const std::queue<T, C>& container)
	{
		if(WriteContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

	template <typename BufferT>
	template<typename T>
	inline NetworkWriter<BufferT>& 
		NetworkWriter<BufferT>::operator <<(const std::deque<T>& container)
	{
		if(WriteContainer(container) == FALSE)
			throw std::exception("");

		return *this;
	}

}