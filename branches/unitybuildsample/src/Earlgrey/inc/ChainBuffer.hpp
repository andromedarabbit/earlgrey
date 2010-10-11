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
		
		typedef basic_buffer<T, A >                         buffer_type;
		typedef buffer_type*                                buffer_pointer;
		typedef const buffer_type*                          buffer_const_pointer;
		typedef buffer_type&                                buffer_reference;
		typedef const buffer_type&                          buffer_const_reference;

		typedef typename buffer_type::size_type             size_type;
		typedef typename buffer_type::allocator_type        allocator_type;
		typedef typename buffer_type::reference             reference;
		typedef typename buffer_type::const_reference       const_reference;
		typedef typename buffer_type::pointer               pointer;
		typedef typename buffer_type::const_pointer         const_pointer;
		typedef typename buffer_type::value_type            value_type;

		typedef std::list<buffer_pointer, A>                buffer_list_type;
		typedef 
			typename allocator_type::rebind<buffer_type>::other 
			buffer_allocator_type;

		typedef std::tr1::tuple<pointer, size_t>			buffer_node_desc_type;
		typedef std::vector<buffer_node_desc_type>			desc_vector_type;
		
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

		reference       operator[](size_type n);
		const_reference operator[](size_type n) const;

		reference       at(size_type n);
		const_reference at(size_type n) const;

		void set(const_pointer ptr, size_type length);
		void append(const_pointer ptr, size_type length);
		void copy_to(chain_buffer& rhs) const;
		bool get(size_type offset, pointer ptr, size_type length);
		void get_descriptions(desc_vector_type& desc_vector);

		
		buffer_node_desc_type expand(size_t size);
		void increase_tail_size(size_t length);

		void clear();
		bool empty() const;
		void erase(size_type offset, size_type length);
		
		iterator begin();
		iterator end();

	private:
		buffer_pointer new_buffer(size_type initial_capacity);

	private:
		allocator_type m_allocator;	
		buffer_list_type m_buffer_list;
		buffer_allocator_type m_buffer_allocator;
	};

	//! \note 생성자 호출시 m_buffer_allocator.construct() 를 이용하는 게 표준이다. 
	// 하지만 construct()는 복사 생성자를 호출하기 때문에 직접 생성자를 호출하는 방식을 택했다.
	template <typename T, typename A>
	inline 
		typename chain_buffer<T,A>::buffer_pointer chain_buffer<T,A>::new_buffer(
			size_type initial_capacity
			)
	{
		// 메모리 할당
		buffer_pointer newBuffer = m_buffer_allocator.allocate(1);
		
		// 생성자 호출
		newBuffer->buffer_type::basic_buffer(initial_capacity, m_allocator);
		return newBuffer;
	}

	template <typename T, typename A>
	inline
		chain_buffer<T,A>::chain_buffer(size_type initial_capacity, const allocator_type &allocator)
		: m_allocator(allocator)
	{
		EARLGREY_ASSERT(initial_capacity > 0);
		m_buffer_list.push_back( new_buffer(initial_capacity) );
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
		size_type capacity = 0;
		for(buffer_list_type::const_iterator it = m_buffer_list.begin(); it != m_buffer_list.end(); it++)
		{
			capacity += (*it)->capacity();
		}
		return capacity;
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::size_type chain_buffer<T,A>::size() const
	{
		size_type size = 0;
		for(buffer_list_type::const_iterator it = m_buffer_list.begin(); it != m_buffer_list.end(); it++)
		{
			size += (*it)->size();
		}
		return size;
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
			size_type cur_size = (*it)->size();
			if(pos < cur_size)
			{
				buffer_pointer found = (*it);
				return (*found)[pos];
			}

			pos = pos - cur_size;
		}

		it--;
		buffer_pointer found = (*it);
		return (*found)[pos];
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
		buffer_pointer lastBuffer = m_buffer_list.back();

		if(lastBuffer->capacity() - lastBuffer->size() < length)
		{
			size_type buffer_size = std::max EARLGREY_PREVENT_MACRO_SUBSTITUTION (length, size() * 2);
			lastBuffer = new_buffer(buffer_size);
			m_buffer_list.push_back(lastBuffer);
		}

		lastBuffer->append(ptr, length);
	}

	template <typename T, typename A>
	inline
		void chain_buffer<T,A>::append(const_pointer ptr, size_type length)
	{
		set( ptr, length );
	}

	template <typename T, typename A>
	inline
		void chain_buffer<T,A>::copy_to(chain_buffer& rhs) const
	{
		rhs.clear();

		for(buffer_list_type::const_iterator it = m_buffer_list.begin(); it != m_buffer_list.end(); it++)
		{
			rhs.append( (*it)->data(), (*it)->size() );
		}
	}

	template <typename T, typename A>
	inline
		void chain_buffer<T,A>::clear()
	{
		for(buffer_list_type::iterator it = m_buffer_list.begin(); it != m_buffer_list.end(); it++)
		{
			// delete *it;
			m_buffer_allocator.destroy(*it);
			m_buffer_allocator.deallocate(*it, 1);
		}
		
		m_buffer_list.clear();
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
			size_type cur_size = (*it)->size();
			if(pos < cur_size)
			{
				buffer_pointer found = (*it);
				size_type size_after_offset = cur_size - pos;
				size_type erase_size = std::min( size_after_offset, length_to_erase );

				found->erase( pos, erase_size );
				length_to_erase -= erase_size;
				pos = 0;

				if (found->size() == 0)
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
		memcpy_s( ptr, length, &(*this)[offset], length );
		return true;
	}

	template <typename T, typename A>
	inline
		typename chain_buffer<T,A>::buffer_node_desc_type
			chain_buffer<T,A>::expand(size_t length)
	{
		buffer_pointer lastBuffer = m_buffer_list.back();
		pointer p = &lastBuffer->at( lastBuffer->size() -1);
		size_t bufSize = lastBuffer->capacity() - lastBuffer->size();

		if(bufSize < length)
		{
			size_type buffer_size = std::max EARLGREY_PREVENT_MACRO_SUBSTITUTION (length, size() * 2);
			lastBuffer = new_buffer(buffer_size);
			m_buffer_list.push_back(lastBuffer);
			p = &lastBuffer->at( 0 );
			bufSize = lastBuffer->capacity();
		}
		return chain_buffer<T,A>::buffer_node_desc_type( p, bufSize );
	}

	template <typename T, typename A>
	inline
		void chain_buffer<T,A>::increase_tail_size(size_t length)
	{
		buffer_pointer lastBuffer = m_buffer_list.back();
		lastBuffer->resize_noset( lastBuffer->size() + length );
	}

	template <typename T, typename A>
	inline
		void chain_buffer<T,A>::get_descriptions(typename chain_buffer<T,A>::desc_vector_type& desc_vector)
	{
		typedef typename chain_buffer<T,A>::buffer_node_desc_type buffer_node_desc_type;

		for(buffer_list_type::iterator it = m_buffer_list.begin(); it != m_buffer_list.end(); it++)
		{
			desc_vector.push_back( buffer_node_desc_type( (*it)->data(), (*it)->size() ) );
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

