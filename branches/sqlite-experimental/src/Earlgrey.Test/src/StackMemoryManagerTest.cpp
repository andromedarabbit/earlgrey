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
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT * 2, manager.m_current_pos);

		pointer = 
			manager.malloc(StackMemoryManager::DEFAULT_ALIGNMENT, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_TRUE(pointer != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT * 3, manager.m_current_pos);

		manager.Unmark(0);
	}


	TEST(StackMemoryManagerTest, UseEveryBitOfInternalMemory) 
	{
		const size_t capacity = StackMemoryManager::DEFAULT_ALIGNMENT * 2;
		StackMemoryManager manager(capacity);
		ASSERT_EQ(0, manager.m_current_pos);
		ASSERT_EQ(capacity, manager.m_buffer_end - manager.m_buffer_begin);

		manager.Mark();

		const size_t bytes = StackMemoryManager::DEFAULT_ALIGNMENT - 1;
		void * pointer = 
			manager.malloc(bytes, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_TRUE(pointer != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT, manager.m_current_pos);

		pointer = 
			manager.malloc(StackMemoryManager::DEFAULT_ALIGNMENT, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_TRUE(pointer != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT * 2, manager.m_current_pos);

		manager.Unmark(0);
	}

	TEST(StackMemoryManagerTest, UseEveryBitOfInternalMemory2) 
	{
		const size_t capacity = StackMemoryManager::DEFAULT_ALIGNMENT * 2;
		StackMemoryManager manager(capacity);
		ASSERT_EQ(0, manager.m_current_pos);
		ASSERT_EQ(capacity, manager.m_buffer_end - manager.m_buffer_begin);

		manager.Mark();

		const size_t bytes = StackMemoryManager::DEFAULT_ALIGNMENT - 1;
		void * pointer = 
			manager.malloc(bytes, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_TRUE(pointer != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT, manager.m_current_pos);

		pointer = 
			manager.malloc(StackMemoryManager::DEFAULT_ALIGNMENT - 1, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_TRUE(pointer != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT * 2, manager.m_current_pos);

		manager.Unmark(0);
	}

	TEST(StackMemoryManagerTest, ZeroSizeAllocation) 
	{
		const size_t capacity = StackMemoryManager::DEFAULT_ALIGNMENT * 2;
		StackMemoryManager manager(capacity);
		ASSERT_EQ(0, manager.m_current_pos);
		ASSERT_EQ(capacity, manager.m_buffer_end - manager.m_buffer_begin);

		manager.Mark();

		const size_t bytes = 0;
		void * pointer = 
			manager.malloc(bytes, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_TRUE(pointer != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT, manager.m_current_pos);

		pointer = 
			manager.malloc(bytes, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_TRUE(pointer != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT * 2, manager.m_current_pos);

		manager.Unmark(0);
	}

	//}
}