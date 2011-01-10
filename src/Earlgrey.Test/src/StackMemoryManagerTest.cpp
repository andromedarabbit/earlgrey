#include "stdafx.h"
#include "StackMemoryManager.h"

// #define EARLGREY_UNIT_TEST

namespace Earlgrey
{
	//namespace Test
	//{

	TEST(StackMemoryManagerTest, Alloc) 
	{
		const size_t capacity = StackMemoryManager::DEFAULT_ALIGNMENT * 5;
		StackMemoryManager manager(capacity);
		ASSERT_EQ(0, manager.m_current_pos);

		manager.Mark();

		void * pointer = 
			manager.malloc(StackMemoryManager::DEFAULT_ALIGNMENT, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_TRUE(pointer != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT, manager.m_current_pos);

		manager.Unmark(0);
	}

	TEST(StackMemoryManagerTest, AllocNotWithAlignmentSize) 
	{
		const size_t capacity = StackMemoryManager::DEFAULT_ALIGNMENT * 10;
		StackMemoryManager manager(capacity);
		ASSERT_EQ(0, manager.m_current_pos);

		manager.Mark();

		const size_t bytes = StackMemoryManager::DEFAULT_ALIGNMENT + 1;
		void * pointer = 
			manager.malloc(bytes, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_TRUE(pointer != NULL);
		ASSERT_EQ(bytes, manager.m_current_pos);

		pointer = 
			manager.malloc(StackMemoryManager::DEFAULT_ALIGNMENT, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_TRUE(pointer != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT * 3, manager.m_current_pos);

		manager.Unmark(0);
	}

	//}
}