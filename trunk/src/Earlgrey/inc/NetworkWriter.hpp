#pragma once
#include "BinaryWriter.hpp"
#include "numeric_cast.hpp"

#include <map>
#include <stack>
#include <queue>

namespace Earlgrey
{
	//! C++은 int, long 등 정수형의 크기와 포맷이 플랫폼이나 컴파일러 구현 등에 맡긴다.
	// 따라서 <inttypes.h> 등을 이용해 플랫폼 간의 상이함을 해결해야 하나 VC++엔 현재 <inttype.h>가 없다.
	template <typename BufferT>
	class NetworkWriter : private Uncopyable
	{
	public:
		typedef BinaryWriter<BufferT> RawWriter;
		typedef typename RawWriter::size_type size_type;
		typedef UINT16 length_type;

		explicit NetworkWriter(BufferT& buffer);
		~NetworkWriter();

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

		size_type Size() const;

		const BufferT& Buffer() const;

		BufferT& Buffer();

		BOOL WriteBytes(const void* buf, size_type len);
	

		template <typename T>
		inline NetworkWriter& operator<<(const T& x)
		{
// 			if(this->Write(x) == FALSE)
// 			{
// 				// TODO
// 				throw std::exception("");
// 			}
			this->Write(x);
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
		template<class T>
			inline BOOL WriteContainer(const T& container)
		{
			const length_type size = EARLGREY_NUMERIC_CAST<length_type>(container.size());
			if(m_BinaryWriter.Write(size) == FALSE)
				return FALSE;

			T::const_iterator it = container.begin();
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
		this->Write(x);
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(const xwstring& x)
	{
		this->Write(x.c_str(), EARLGREY_NUMERIC_CAST<length_type>(x.length()));
		return *this;
	}

	template <typename BufferT>
	inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(const CHAR * x)
	{
		this->WriteString(x);
		return *this;
	}

	template <typename BufferT>
		inline NetworkWriter<BufferT>& NetworkWriter<BufferT>::operator<<(const xstring& x)
	{
		this->WriteString(x.c_str(), EARLGREY_NUMERIC_CAST<length_type>(x.length()));
		return *this;
	}

	template <typename BufferT>
	template<class Kty, class Ty, class Pr, class Alloc>
	inline NetworkWriter<BufferT>& 
		NetworkWriter<BufferT>::operator <<(const std::map<Kty, Ty, Pr, Alloc>& container)
	{
		WriteContainer(container);
		return *this;
	}

	template <typename BufferT>
	template<typename T, typename Alloc>
	inline NetworkWriter<BufferT>& 
		NetworkWriter<BufferT>::operator <<(const std::vector<T, Alloc>& container)
	{
		WriteContainer(container);
		return *this;
	}

	template <typename BufferT>
	template<typename T, typename C>
	inline NetworkWriter<BufferT>& 
		NetworkWriter<BufferT>::operator <<(const std::stack<T, C>& container)
	{
		WriteContainer(container);
		return *this;
	}

	template <typename BufferT>
	template<class Kty, class Pr, class Alloc>
	inline NetworkWriter<BufferT>& 
		NetworkWriter<BufferT>::operator <<(const std::set<Kty, Pr, Alloc>& container)
	{
		WriteContainer(container);
		return *this;
	}

	template <typename BufferT>
	template<typename T, typename C>
	inline NetworkWriter<BufferT>&
		NetworkWriter<BufferT>::operator <<(const std::queue<T, C>& container)
	{
		WriteContainer(container);
		return *this;
	}

	template <typename BufferT>
	template<typename T>
	inline NetworkWriter<BufferT>& 
		NetworkWriter<BufferT>::operator <<(const std::deque<T>& container)
	{
		WriteContainer(container);
		return *this;
	}

}