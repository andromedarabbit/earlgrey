#pragma once
#include "Uncopyable.h"
#include "StackMemoryManager.h"

namespace Earlgrey
{
	//! \todo ���� �Ҵ� ���ϰ� �ؾ� �Ѵ�. Effective C++ 2��.
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
		static void * operator new(size_t size); // �� ������ ������� �ʴ´�.s
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