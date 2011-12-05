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
		StackMemoryManager manager(capacity, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_EQ(0, manager.m_current_pos);

		manager.Mark();

		void * pointer = 
			manager.malloc(StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_TRUE(pointer != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT, manager.m_current_pos);
		manager.free(pointer);

		manager.Unmark(0);
	}

	TEST(StackMemoryManagerTest, AllocByteAndByte) 
	{
		const size_t capacity = StackMemoryManager::DEFAULT_ALIGNMENT * 10;
		StackMemoryManager manager(capacity, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_EQ(0, manager.m_current_pos);

		manager.Mark();

		const size_t bytes = 1;
		void * pointer1 = 
			manager.malloc(bytes);
		ASSERT_TRUE(pointer1 != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT, manager.m_current_pos);

		void * pointer2 = 
			manager.malloc(1);
		ASSERT_TRUE(pointer2 != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT * 2, manager.m_current_pos);

		manager.free(pointer2);
		manager.free(pointer1);

		manager.Unmark(0);
	}

	TEST(StackMemoryManagerTest, AllocNotWithAlignmentSize) 
	{
		const size_t capacity = StackMemoryManager::DEFAULT_ALIGNMENT * 10;
		StackMemoryManager manager(capacity, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_EQ(0, manager.m_current_pos);

		manager.Mark();

		const size_t bytes = StackMemoryManager::DEFAULT_ALIGNMENT + 1;
		void * pointer1 = 
			manager.malloc(bytes);
		ASSERT_TRUE(pointer1 != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT * 2, manager.m_current_pos);

		void * pointer2 = 
			manager.malloc(StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_TRUE(pointer2 != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT * 3, manager.m_current_pos);

		manager.free(pointer2);
		manager.free(pointer1);

		manager.Unmark(0);
	}

	TEST(StackMemoryManagerTest, UseEveryBitOfInternalMemory) 
	{
		const size_t capacity = StackMemoryManager::DEFAULT_ALIGNMENT * 2;
		StackMemoryManager manager(capacity, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_EQ(0, manager.m_current_pos);
		ASSERT_EQ(capacity, manager.m_buffer_end - manager.m_buffer_begin);

		manager.Mark();

		const size_t bytes = StackMemoryManager::DEFAULT_ALIGNMENT - 1;
		void * pointer1 = 
			manager.malloc(bytes);
		ASSERT_TRUE(pointer1 != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT, manager.m_current_pos);	

		void * pointer2 = 
			manager.malloc(StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_TRUE(pointer2 != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT * 2, manager.m_current_pos);

		manager.free(pointer2);
		manager.free(pointer1);

		manager.Unmark(0);
	}

	TEST(StackMemoryManagerTest, UseEveryBitOfInternalMemory2) 
	{
		const size_t capacity = StackMemoryManager::DEFAULT_ALIGNMENT * 2;
		StackMemoryManager manager(capacity, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_EQ(0, manager.m_current_pos);
		ASSERT_EQ(capacity, manager.m_buffer_end - manager.m_buffer_begin);

		manager.Mark();

		const size_t bytes = StackMemoryManager::DEFAULT_ALIGNMENT - 1;
		void * pointer1 = 
			manager.malloc(bytes);
		ASSERT_TRUE(pointer1 != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT, manager.m_current_pos);

		void * pointer2 = 
			manager.malloc(StackMemoryManager::DEFAULT_ALIGNMENT - 1);
		ASSERT_TRUE(pointer2 != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT * 2, manager.m_current_pos);

		manager.free(pointer2);
		manager.free(pointer1);

		manager.Unmark(0);
	}

	TEST(StackMemoryManagerTest, ZeroSizeAllocation) 
	{
		const size_t capacity = StackMemoryManager::DEFAULT_ALIGNMENT * 2;
		StackMemoryManager manager(capacity, StackMemoryManager::DEFAULT_ALIGNMENT);
		ASSERT_EQ(0, manager.m_current_pos);
		ASSERT_EQ(capacity, manager.m_buffer_end - manager.m_buffer_begin);

		manager.Mark();

		const size_t bytes = 0;
		void * pointer1 = 
			manager.malloc(bytes);
		ASSERT_TRUE(pointer1 != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT, manager.m_current_pos);

		void * pointer2 = 
			manager.malloc(bytes);
		ASSERT_TRUE(pointer2 != NULL);
		ASSERT_EQ(StackMemoryManager::DEFAULT_ALIGNMENT * 2, manager.m_current_pos);

		manager.free(pointer2);
		manager.free(pointer1);

		manager.Unmark(0);
	}

	//}
}