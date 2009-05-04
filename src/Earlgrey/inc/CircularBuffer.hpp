#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"
#include "StlCustomAllocator.hpp"
#include <memory> // std::autoptr

#undef min
#undef max

namespace Earlgrey
{
	// template <typename T, typename A = StlCustomAllocator<T> > //< See note below
	template <typename T, typename A = std::allocator<T> > //< See note below
	class circular_buffer : private Uncopyable
	{
	public:
		typedef T                                        value_type;
		typedef A                                        allocator_type;
		typedef circular_buffer<T,A>                     self_type;
		typedef typename allocator_type::difference_type difference_type;
		typedef typename allocator_type::reference       reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::pointer         pointer;
		typedef typename allocator_type::const_pointer   const_pointer;
	
		typedef size_t size_type;


	public:
		explicit circular_buffer(size_t capacity, const allocator_type &allocator = allocator_type());

		~circular_buffer();

		inline allocator_type get_allocator() const;

		inline size_type size() const
		{
			if(empty())
				return 0;

			return (m_back > m_front ? m_back : m_back+m_capacity) - m_front;
		}

		inline size_type max_size() const;
		
		inline bool empty() const
		{
			return !m_front;
		}

		inline bool full() const 
		{ 
			return capacity() == size(); 
		}

		inline size_type capacity() const
		{
			return m_capacity;
		}

		reference       front();
		const_reference front() const;

		bool push_back(const value_type &value);
		void pop_front();

		void clear();

	private:
		value_type *wrap(value_type *ptr) const
		{
			assert(ptr < m_buffer + m_capacity*2);
			if (ptr >= m_buffer+m_capacity)
				return ptr-m_capacity;
			else
				return ptr;
		}

	private:		
		size_type      m_capacity;
		allocator_type m_allocator;
		pointer        m_buffer;
		pointer        m_front;
		pointer        m_back;
	};

	template <typename T, typename A>
	inline
		circular_buffer<T,A>::circular_buffer
		(size_type capacity, const allocator_type &allocator)
		: m_capacity(capacity),
		m_allocator(allocator),
		m_buffer(m_allocator.allocate(capacity)),
		m_front(0),
		m_back(m_buffer)
	{
	}

	template <typename T, typename A>
	inline
		circular_buffer<T,A>::~circular_buffer()
	{
		clear(); // Delete all objects before deallocating the buffer
		m_allocator.deallocate(m_buffer, m_capacity);
	}

	template <typename T, typename A>
	typename circular_buffer<T,A>::allocator_type
		inline
		circular_buffer<T,A>::get_allocator() const
	{
		return m_allocator;
	}

	template <typename T, typename A>
	inline
		typename circular_buffer<T,A>::size_type circular_buffer<T,A>::max_size() const
	{
		// This is clearly more elegant than the previous implementation!
		return m_allocator.max_size();
	}

	// This version of push_back must constuct and detroy objects in m_buffer
	// using m_allocators methods, rather than traditional construction, copying,
	// assignment.
	template <typename T, typename A>
	inline
		bool circular_buffer<T,A>::push_back(const value_type &value)
	{
		// If the buffer is full, and data will fall off the back of the
		// buffer - so we must destroy the stale object first.
		// This is an implementation choice, we could instead use operator= on
		// the value_type to replace the item. However, that would require
		// value_type to provide a copy assignment operator.
		if (m_front && m_front == m_back)
			m_allocator.destroy(m_back);

		m_allocator.construct(m_back, value);

		// The rest of push_back is as it was before
		value_type *const next = wrap(m_back+1);
		if (!m_front)
		{
			// first entry in the buffer
			m_front = m_back;
			m_back = next;
			return true;
		}
		else if (m_front == m_back)
		{
			// buffer is full already, throw something away
			m_front = m_back = next;
			return false;
		}
		else
		{
			m_back = next;
			return true;
		}
	}

	template <typename T, typename A>
	inline
		void circular_buffer<T,A>::pop_front()
	{
		assert(m_front);

		m_allocator.destroy(m_front);
		value_type *const next = wrap(m_front+1);
		if (next == m_back)
			m_front = 0;
		else
			m_front = next;
	}

	template <typename T, typename A>
	inline
		void circular_buffer<T,A>::clear()
	{
		if (m_front)
		{
			do
			{
				m_allocator.destroy(m_front);
				m_front = wrap(m_front+1);
			}
			while (m_front != m_back);
		}
		m_front = 0;
	}
}