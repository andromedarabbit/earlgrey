#pragma once 
#include "EarlgreyAssert.h"

#include "StlCustomAllocator.hpp"
#include "BasicBufferIterator.hpp"

namespace Earlgrey
{
	class Uncopyable;

	//! \note 2의 배수로 메모리 할당 받기? -> allocator에서 할 일?
	template <typename T> // , typename A = StlDefaultAllocator<T>::Type >
	class simple_buffer : private Uncopyable
	{
	public:	
		typedef T                                            value_type;
		/*
		typedef A                                            allocator_type;
		typedef typename allocator_type::size_type           size_type;
		typedef typename allocator_type::difference_type     difference_type;
		typedef typename allocator_type::reference           reference;
		typedef typename allocator_type::const_reference     const_reference;
		typedef typename allocator_type::pointer             pointer;
		typedef typename allocator_type::const_pointer       const_pointer;
		*/
		typedef typename T value_type;
		typedef value_type _FARQ *pointer;
		typedef value_type _FARQ& reference;
		typedef const value_type _FARQ *const_pointer;
		typedef const value_type _FARQ& const_reference;

		typedef _SIZT size_type;
		typedef _PDFT difference_type;

		// class                                                iterator;
		typedef 
			typename basic_buffer_iterator< simple_buffer<T> > 
			iterator;

		/*
		typedef simple_buffer<T, A>                          buffer_type;
		typedef simple_buffer<T, A>*                         buffer_pointer;
		typedef const simple_buffer<T, A>*                   buffer_const_pointer;
		typedef simple_buffer<T, A>&                         buffer_reference;
		typedef const simple_buffer<T, A>&                   buffer_const_reference;
		*/
		// static const size_type DEFAULT_INITIAL_BUFFERSIZE = 16;

		// explicit simple_buffer(size_type initialCapacity = DEFAULT_INITIAL_BUFFERSIZE, const allocator_type &a = allocator_type());
		explicit simple_buffer(T* buffer, size_type capacity, size_type size = 0);
		~simple_buffer();
		// allocator_type get_allocator() const;

		size_type capacity() const;
		size_type size() const;

		reference front();
		const_reference front() const;

		reference back();
		const_reference back() const;

		pointer data();
		const_pointer data() const;

		reference       operator[](size_type n);
		const_reference operator[](size_type n) const;

		reference       at(size_type n);
		const_reference at(size_type n) const;

		void set(const_pointer ptr, size_type length);
		void append(const_pointer ptr, size_type length);

		void clear();
		bool empty() const;

		iterator begin();
		iterator end();

		void reserve(size_type length);
		void resize (size_type length, T obj = T());


	private:
		T* m_buffer;		
		size_type m_capacity;
		size_type m_size;

		// allocator_type m_allocator;

		// buffer_pointer m_next;
	};


	template <typename T> //, typename A>
	inline
		simple_buffer<T>::simple_buffer(T* buffer, size_type capacity, size_type size)
		: m_capacity(capacity)
		// , m_allocator(allocator)
		, m_buffer(buffer)		 
		, m_size(size)
		// , m_next(NULL)
	{
		EARLGREY_ASSERT(buffer != NULL);
		EARLGREY_ASSERT(m_capacity > 0);
	}

	/*
	template <typename T>
	inline
		simple_buffer<T>::simple_buffer(size_type initial_capacity, const allocator_type &allocator)
		: m_capacity(initial_capacity)
		, m_allocator(allocator)
		, m_buffer(m_allocator.allocate(initial_capacity))		 
		, m_size(0)
		// , m_next(NULL)
	{
		EARLGREY_ASSERT(initial_capacity > 0);
	}
	*/

	template <typename T>
	inline
		simple_buffer<T>::~simple_buffer()
	{
		clear(); // deallocates all objects
		// m_allocator.deallocate(m_buffer, m_capacity);

	}

	template <typename T>
	inline
		typename simple_buffer<T>::size_type simple_buffer<T>::capacity() const
	{
		return m_capacity;
	}

	template <typename T>
	inline
		typename simple_buffer<T>::size_type simple_buffer<T>::size() const
	{
		return m_size;		
	}

	template <typename T>
	inline
		typename simple_buffer<T>::reference simple_buffer<T>::front() 
	{ 
		return *m_buffer; 
	}

	template <typename T>
	inline
		typename simple_buffer<T>::const_reference simple_buffer<T>::front() const 
	{ 
		return *m_buffer; 
	}

	template <typename T>
	inline
		typename simple_buffer<T>::reference simple_buffer<T>::back() 
	{ 
		return (*this)[m_size - 1]; 
	}

	template <typename T>
	inline
		typename simple_buffer<T>::const_reference simple_buffer<T>::back() const 
	{ 
		return (*this)[m_size - 1]; 
	}

	template <typename T>
	inline
		typename simple_buffer<T>::pointer simple_buffer<T>::data() 
	{ 
		return m_buffer; 
	}

	template <typename T>
	inline
		typename simple_buffer<T>::const_pointer simple_buffer<T>::data() const 
	{ 
		return m_buffer; 
	}



	template <typename T>
	inline
		typename simple_buffer<T>::reference simple_buffer<T>::operator[] (size_type n)
	{
		if (n >= size()) throw std::out_of_range("Parameter out of range");
		return *(m_buffer + n);
	}

	template <typename T>
	inline
		typename simple_buffer<T>::const_reference simple_buffer<T>::operator[]	(size_type n) const
	{
		if (n >= size()) throw std::out_of_range("Parameter out of range");
		return *(m_buffer + n);
	}


	template <typename T>
	inline
		typename simple_buffer<T>::reference simple_buffer<T>::at(size_type n)
	{
		return (*this)[n];
	}

	template <typename T>
	inline
		typename simple_buffer<T>::const_reference simple_buffer<T>::at
		(size_type n) const
	{
		return (*this)[n];
	}

	template <typename T>
	inline
		void simple_buffer<T>::set(const_pointer ptr, size_type length)
	{
		if (length > capacity()) throw std::out_of_range("Parameter out of range");		

		// TODO: _CRT_SECURE_DEPRECATE_MEMORY 를 정의하면 memcpy는 안전하지 못하기 때문에 쓰지 못하게 한다.
		memcpy( m_buffer, ptr, length * sizeof(T) );
		m_size = length;
	}

	template <typename T>
	inline
		void simple_buffer<T>::append(const_pointer ptr, size_type length)
	{
		if (m_size + length > capacity()) throw std::out_of_range("Parameter out of range");

		memcpy( m_buffer + m_size, ptr, length * sizeof(T) );
		m_size += length;
	}

	/*
	template <typename T>
	inline
		void simple_buffer<T>::reserve(size_type length)
	{
		// throw std::exception("not yet implemented!");
		if (length <= capacity())
			return;

		T* newBuffer = m_allocator.allocate(length);
		EARLGREY_ASSERT(newBuffer != NULL);

		EARLGREY_ASSERT(memmove_s(newBuffer, length * sizeof(T), m_buffer, m_size * sizeof(T)) == 0);

		T* oldBuffer = m_buffer;
		m_buffer = newBuffer;

		m_capacity = length;

		m_allocator.deallocate(oldBuffer, capacity());
	}

	template <typename T> 
	inline
		void simple_buffer<T>::resize (size_type length, T obj = T())
	{
		if(length == capacity())
			return;

		if (length < capacity())
		{
			for(size_type i = length; i < m_size; i++)
			{
				// call destructor of T class
				m_allocator.destroy(m_buffer + i);
			}

			m_size = length;
			return;
		}

		reserve(length);
		const size_type numberOfNewItems = length - m_size;
		for(size_type i = 0; i < numberOfNewItems; i++)
		{
			append(&obj, 1);
		}
		m_size = length;
	}
	*/

	template <typename T>
	inline
		void simple_buffer<T>::clear()
	{
		/*
		for(size_type i = 0; i < m_size; i++)
		{
			// call destructor of T class
			m_allocator.destroy(m_buffer + i);
		}
		*/

		m_size = 0;
	}

	template <typename T>
	inline
		bool simple_buffer<T>::empty() const
	{
		return size() == 0;
	}

	template <typename T>
	typename simple_buffer<T>::iterator simple_buffer<T>::begin()
	{
		return iterator(*this, 0);
	}

	template <typename T>
	typename simple_buffer<T>::iterator simple_buffer<T>::end()
	{
		return iterator(*this, size());
	}

}

