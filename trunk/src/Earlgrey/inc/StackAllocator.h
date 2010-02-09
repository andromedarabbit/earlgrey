#pragma once
#include "Uncopyable.h"
#include "StackMemoryManager.h"

namespace Earlgrey
{
	//! \todo 힙에 할당 못하게 해야 한다. Effective C++ 2권.
	class StackAllocator : private Uncopyable
	{
	public:
		typedef StackMemoryManager::size_type size_type;

		explicit StackAllocator()
			: m_stack_pos(gStackMemoryManager::Instance().m_current_pos)
		{
			gStackMemoryManager::Instance().Mark();
		}

		StackAllocator(const StackAllocator& allocator)
			: m_stack_pos(gStackMemoryManager::Instance().m_current_pos)
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
			Pop();
		}

		inline void * malloc(size_type size, size_type alignment = StackMemoryManager::DEFAULT_ALIGNMENT)
		{
			return gStackMemoryManager::Instance().malloc(size, alignment);			
		}

		inline void free(void * memblock)
		{
			gStackMemoryManager::Instance().free(memblock);
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
}