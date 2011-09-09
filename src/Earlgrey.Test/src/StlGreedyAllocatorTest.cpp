#include "stdafx.h"
#include "StlGreedyAllocator.hpp"

namespace Earlgrey
{
	namespace Test
	{
		using std::tr1::shared_ptr;

		TEST(StlGreedyAllocatorTest, Basic)
		{
			Earlgrey::StlGreedyAllocator<int> alloc;
			int* p = (int*)alloc.allocate(1);
			alloc.deallocate(p, sizeof(int));

		}

		TEST(StlGreedyAllocatorTest, UsingDeleter)
		{
			Earlgrey::StlGreedyAllocator<int> alloc;
			shared_ptr<int> p = shared_ptr<int>( 
				alloc.allocate(1) 
				, Earlgrey::StlGreedyAllocator<int>::Deleter()
				);
		}
	}
}