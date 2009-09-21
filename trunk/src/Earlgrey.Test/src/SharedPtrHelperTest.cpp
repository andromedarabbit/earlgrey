#include "stdafx.h"
#include "shared_ptr_helper.h"

namespace Earlgrey
{
	namespace Test
	{		
		TEST(SharedPtrHelperTest, Basic)
		{
			Earlgrey::StlGreedyAllocator<int> alloc;
			std::tr1::shared_ptr<int> p = Earlgrey::create_shared_ptr<int>( alloc.allocate(1) );
			ASSERT_NE( p.get(), static_cast<int*>(NULL) );

			std::tr1::shared_ptr<int> p2 = p;
			p2.reset( alloc.allocate(1), Earlgrey::StlCustomDeletor<int>(), Earlgrey::StlDefaultAllocator<int>::Type() );
		}
	}
}