#pragma once
#include "Uncopyable.h"
#include "StackMemoryManager.h"

#ifdef EARLGREY_UNIT_TEST
#include <gtest/gtest_prod.h>
#endif


namespace Earlgrey
{
	//! \todo 힙에 할당 못하게 해야 한다. Effective C++ 2권.
	class StackAllocator : private Uncopyable
	{
#ifdef EARLGREY_UNIT_TEST
		friend class StackAllocatorTest;
		FRIEND_TEST(StackAllocatorTest, CalculateAlignment);
#endif

	public:
		typedef StackMemoryManager::size_type size_type;

		explicit StackAllocator()
			: m_stack_pos(gStackMemoryManager::Instance().m_current_pos)
		{
			gStackMemoryManager::Instance().Mark();
		}

		StackAllocator(const StackAllocator& allocator)
			: m_stack_pos(gStackMemoryManager::Instance().m_current_pos)
			// : m_stack_pos(allocator.m_stack_pos)
		{
			UNREFERENCED_PARAMETER(allocator);
			EARLGREY_ASSERT(allocator.m_stack_pos <= this->m_stack_pos);
			gStackMemoryManager::Instance().Mark();
		}

		/*
		StackAllocator& operator = (const StackAllocator& allocator)
		{
			m_stack_pos = allocator.m_stack_pos;
			return (*this);
		}
		*/

		~StackAllocator()
		{
			// Pop();
		}

		inline void * malloc(size_type size)
		{
			return gStackMemoryManager::Instance().malloc(size);			
		}

		inline void free(void * memblock)
		{
			gStackMemoryManager::Instance().free(memblock);
		}

		inline void free(void * memblock, size_t bytes)
		{
			gStackMemoryManager::Instance().free(memblock, bytes);
		}

	private:
		static void * operator new(size_t size); // 힙 생성은 허용하지 않는다.s
		static void operator delete(void *ptr);
		static void *operator new[](size_t size);
		static void operator delete[](void *ptr);

		inline void Pop()
		{
			gStackMemoryManager::Instance().Unmark(m_stack_pos);
		}

	private:
		size_type m_stack_pos;
	};

	// #define EARLGREY_ALLOA(bytes) \
	//	Earlgrey::StackAllocator stackAlloc; 

	//void *Malloca(const size_t size);
	//void Freea(void *memblock);


	template<typename T>
	inline 
		T * Malloca(const size_t length)
	{
		return reinterpret_cast<T*>(
			gStackMemoryManager::Instance().malloc(length * sizeof(T))
			);
	}


	inline 
		void * Malloca(const size_t size)
	{
		// gStackMemoryManager::Instance().Mark();
		return gStackMemoryManager::Instance().malloc(size);
	}

	template<typename T>
	inline 
		void Freea(T * memblock)
	{
		// gStackMemoryManager::Instance().Unmark();
		gStackMemoryManager::Instance().free(memblock);
	}


	inline 
		void Freea(void * memblock)
	{
		// gStackMemoryManager::Instance().Unmark();
		gStackMemoryManager::Instance().free(memblock);
	}

}