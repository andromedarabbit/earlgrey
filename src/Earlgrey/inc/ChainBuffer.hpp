#pragma once 
#include "Uncopyable.h"
#include "EarlgreyAssert.h"
#include "Macros.h"

#include "StlCustomAllocator.hpp"
#include "BasicBuffer.hpp"

#include <list>
#include <tuple>

namespace Earlgrey
{
	//! \note 2의 배수로 메모리 할당 받기? -> allocator에서 할 일?
	//! \todo resize() 구현하기
	//! \todo max_size() 구현하기
	template <
		typename T
		, typename A = StlDefaultAllocator<T>::Type
	>
	class chain_buffer : private Uncopyable
	{
	public:	
		typedef 
			typename basic_buffer_iterator< chain_buffer<T, A> > 
			iterator;
		
		typedef std::vector<T, A>							buffer_type;

		typedef typename buffer_type::size_type             size_type;
		typedef typename buffer_type::allocator_type        allocator_type;
		typedef typename buffer_type::reference             reference;
		typedef typename buffer_type::const_reference       const_reference;
		typedef typename buffer_type::pointer               pointer;
		typedef typename buffer_type::const_pointer         const_pointer;
		typedef typename buffer_type::value_type            value_type;

		typedef std::list<buffer_type, A>					buffer_list_type;

		typedef std::tr1::tuple<pointer, size_t>			buffer_node_desc_type;
		typedef std::vector<buffer_node_desc_type, A>		desc_vector_type;
		
		static const size_type DEFAULT_INITIAL_BUFFERSIZE = 16;

		explicit chain_buffer(size_type initialCapacity = DEFAULT_INITIAL_BUFFERSIZE, const allocator_type &a = allocator_type());
		~chain_buffer();
		allocator_type get_allocator() const;

		size_type capacity() const;
		size_type size() const;
		size_type chain_size() const;

		reference front();
		const_reference front() const;

		reference back();
		const_reference back() const;

		buffer_type* front_buffer();
		buffer_type* back_buffer();
		void pop_front_buffer();
		void pop_back_buffer();

		reference       operator[](size_type n);
		const_reference operator[](size_type n) const;

		reference       at(size_type n);
		const_reference at(size_type n) const;

		void set(const_pointer ptr, size_type length);
		void append(const_pointer ptr, size_type length);
		void increase_size(size_type length);
		void copy_to(chain_buffer& rhs) const;
		bool get(size_type offset, pointer ptr, size_type length);
		void get_descriptions(desc_vector_type& desc_vector);

		desc_vector_type expand(size_t size);

		void clear();
		bool empty() const;
		void erase(size_type offset, size_type length);
		
		iterator begin();
		iterator end();

	private:
		typename buffer_list_type::pointer _new_buffer(size_t length);

	private:
		allocator_type m_allocator;	
		buffer_list_type m_buffer_list;
		size_type m_size;
		size_type m_capacity;
	};

	template <typename T, typename A>
	inline typename chain_buffer<T,A>::buffer_list_type::pointer chain_buffer<T,A>::_new_buffer(size_t length)
	{
		m_buffer_list.push_back( buffer_type() );

		buffer_list_type::pointer buffer = &(m_buffer_list.back());
		buffer->resize( length );
		m_capacity += length;

		return buffer;
	}

	template <typename T, typename A>
	inline
		chain_buffer<T,A>::chain_buffer(size_type initial_capacity, const allocator_type &allocator)
		: m_allocator(allocator), m_size(0), m_capacity(0)
	{
		EARLGREY_ASSERT(initial_capacity > 0);
		_new_buffer( initial_capacity );
	}

	template <typename T, typename A>
	inline
		chain_buffer<T,A>::~chain_buffer()
	{
		clear(); // deallocates all objects
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::size_type chain_buffer<T,A>::capacity() const
	{
		return m_capacity;
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::size_type chain_buffer<T,A>::size() const
	{
		return m_size;
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::size_type chain_buffer<T,A>::chain_size() const
	{
		return m_buffer_list.size();
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::reference chain_buffer<T,A>::front() 
	{ 
		return (*this)[0]; 
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::const_reference chain_buffer<T,A>::front() const 
	{ 
		return (*this)[0];
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::reference chain_buffer<T,A>::back() 
	{ 
		return (*this)[size() - 1]; 
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::const_reference chain_buffer<T,A>::back() const 
	{ 
		return (*this)[size() - 1]; 
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::buffer_type* chain_buffer<T,A>::front_buffer()
	{
		return &(m_buffer_list.front());
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::buffer_type* chain_buffer<T,A>::back_buffer()
	{
		return &(m_buffer_list.back());
	}

	template <typename T, typename A>
	inline void chain_buffer<T,A>::pop_front_buffer()
	{
		size_type size = m_buffer_list.front().size();
		if (m_size < size)
		{
			m_size = 0;
		}
		else 
		{
			m_size -= size;
		}
		m_capacity -= size;
		
		m_buffer_list.pop_front();
	}

	template <typename T, typename A>
	inline void chain_buffer<T,A>::pop_back_buffer()
	{
		size_type size = m_buffer_list.back().size();
		size_type remainder = capacity() - size();
		if (remainder < size)
		{
			m_size -= (size - remainder);
		}
		else
		{
			m_size -= size;
		}
		m_capacity -= size;

		m_buffer_list.pop_back();
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::reference chain_buffer<T,A>::operator[] (size_type n)
	{
		return const_cast<typename chain_buffer<T,A>::reference>(
				(static_cast<const chain_buffer<T,A>&>(*this))[n]
			);
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::const_reference chain_buffer<T,A>::operator[]	(size_type n) const
	{
		if (n >= size()) throw std::out_of_range("Parameter out of range");

		size_type pos = n;

		buffer_list_type::const_iterator it = m_buffer_list.begin();
		for(; it != m_buffer_list.end(); it++)
		{
			size_type cur_size = it->size();
			if(pos < cur_size)
			{
				break;
			}

			pos = pos - cur_size;
		}

		return (*it)[pos];
	}


	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::reference chain_buffer<T,A>::at(size_type n)
	{		
		return (*this)[n];
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::const_reference chain_buffer<T,A>::at
		(size_type n) const
	{		
		return (*this)[n];
	}

	template <typename T, typename A>
	inline
		void chain_buffer<T,A>::set(const_pointer ptr, size_type length)
	{
		size_type remainder = length, bufRemainder = capacity() - size();
		buffer_list_type::pointer buffer = &(m_buffer_list.back());
		EARLGREY_ASSERT(capacity() >= size());
		if (0 == bufRemainder)
		{
			size_type buffer_size = std::max EARLGREY_PREVENT_MACRO_SUBSTITUTION (length, size() * 2);
			buffer = _new_buffer( buffer_size );
			memcpy_s( &(*buffer)[0], buffer_size * sizeof(T), ptr, length * sizeof(T) );
			m_size += length;
			return;
		}

		if (length <= bufRemainder)
		{
			memcpy_s( &(*buffer)[buffer->size() - bufRemainder], bufRemainder * sizeof(T), ptr, length * sizeof(T) );
			m_size += length;
			return;
		}

		memcpy_s( &(*buffer)[buffer->size() - bufRemainder], bufRemainder * sizeof(T), ptr, bufRemainder * sizeof(T) );
		remainder -= bufRemainder;
		m_size += bufRemainder;

		size_type buffer_size = std::max EARLGREY_PREVENT_MACRO_SUBSTITUTION (length, size() * 2);
		buffer = _new_buffer( buffer_size );
		memcpy_s( &(*buffer)[0], buffer_size * sizeof(T), ptr + bufRemainder, remainder * sizeof(T) );
		m_size += remainder;
	}

	template <typename T, typename A>
	inline
		void chain_buffer<T,A>::append(const_pointer ptr, size_type length)
	{
		set( ptr, length );
	}

	template <typename T, typename A>
	inline
		void chain_buffer<T,A>::increase_size(size_type length)
	{
		if (size() + length > capacity())
		{
			throw std::out_of_range("Parameter out of range");
		}
		m_size += length;
	}

	template <typename T, typename A>
	inline
		void chain_buffer<T,A>::copy_to(chain_buffer& rhs) const
	{
		rhs.clear();

		size_type remainder = m_size;

		for(buffer_list_type::const_iterator it = m_buffer_list.begin(); 
			it != m_buffer_list.end(); 
			it++)
		{
			rhs.append( &(*it)[0], (remainder -= std::min EARLGREY_PREVENT_MACRO_SUBSTITUTION( it->size(), remainder )) );
		}
	}

	template <typename T, typename A>
	inline
		void chain_buffer<T,A>::clear()
	{
		m_buffer_list.clear();
		m_size = 0;
		m_capacity = 0;
	}

	template <typename T, typename A>
	inline 
		void chain_buffer<T,A>::erase(size_type offset, size_type length)
	{
		size_type pos = offset;
		size_type length_to_erase = length;

		buffer_list_type::const_iterator it = m_buffer_list.begin();
		while(it != m_buffer_list.end())
		{
			size_type cur_size = it->size();
			if(pos < cur_size)
			{
				size_type size_after_offset = cur_size - pos;
				size_type erase_size = std::min( size_after_offset, length_to_erase );

				it->erase( pos, erase_size );
				length_to_erase -= erase_size;
				pos = 0;

				if (it->size() == 0)
				{
					it = m_buffer_list.erase( it );
				}

				if (length_to_erase)
				{
					continue;
				}
				break;
			}

			pos = pos - cur_size;
			it++;
		}
	}

	template <typename T, typename A>
	inline 
		bool chain_buffer<T,A>::get(size_type offset, pointer ptr, size_type length)
	{
		if (size() < offset + length)
		{
			return false;
		}

		size_type pos = offset, remainder = length;
		size_type cur_size = 0, size_to_copy = 0;
		buffer_list_type::const_iterator it = m_buffer_list.begin();
		for (; it != m_buffer_list.end(); it++)
		{
			cur_size = it->size();
			if (pos >= cur_size)
			{
				pos -= cur_size;
				continue;
			}

			size_to_copy = std::min EARLGREY_PREVENT_MACRO_SUBSTITUTION (cur_size-pos, remainder);
			memcpy_s( ptr + length - remainder, remainder * sizeof(T), &(*it)[pos], size_to_copy * sizeof(T) );

			remainder -= size_to_copy;

			if (0 == remainder)
			{
				break;
			}
			pos = 0;
		}

		return true;
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::desc_vector_type
			chain_buffer<T,A>::expand(size_t length)
	{
		desc_vector_type desc_list;
		size_type remainder = length;
		size_type buf_remainder = capacity() - size();

		if (0 == buf_remainder)
		{
			buffer_list_type::pointer buffer = _new_buffer( length );
			desc_list.push_back( buffer_node_desc_type( &(*buffer)[0], length ) );
		}
		else 
		{
			buffer_list_type::pointer lastBuffer = &(m_buffer_list.back());
			pointer p = &(*lastBuffer)[lastBuffer->size() - buf_remainder];

			desc_list.push_back( buffer_node_desc_type( p, buf_remainder ) );

			if (remainder > buf_remainder)
			{
				remainder -= buf_remainder;
				buffer_list_type::pointer buffer = _new_buffer( remainder );
				desc_list.push_back( buffer_node_desc_type( &(*buffer)[0], remainder ) );
			}
		}
		
		return desc_list;
	}

	template <typename T, typename A>
	inline
		void chain_buffer<T,A>::get_descriptions(typename chain_buffer<T,A>::desc_vector_type& desc_vector)
	{
		typedef typename chain_buffer<T,A>::buffer_node_desc_type buffer_node_desc_type;

		for(buffer_list_type::iterator it = m_buffer_list.begin(); it != m_buffer_list.end(); it++)
		{
			desc_vector.push_back( buffer_node_desc_type( &(*it)[0], it->size() ) );
		}
	}

	template <typename T, typename A>
	inline
		bool chain_buffer<T,A>::empty() const
	{
		return size() == 0;
	}

	template <typename T, typename A>
	typename chain_buffer<T,A>::iterator chain_buffer<T,A>::begin()
	{
		return iterator(*this, 0);
	}

	template <typename T, typename A>
	typename chain_buffer<T,A>::iterator chain_buffer<T,A>::end()
	{
		return iterator(*this, size());
	}
		
}

