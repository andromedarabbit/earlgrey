#include "stdafx.h"
#include "shared_ptr_helper.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(SharedPtrHelperTest, Basic)
		{
			class A
			{
			public:
				A() : i(0) {}
				~A() 
				{
					i = 100;
				}
				int i;
			};

			Earlgrey::StlGreedyAllocator<int> alloc;
			std::tr1::shared_ptr<int> p = Earlgrey::create_shared_ptr<int>( alloc.allocate(1) );
			ASSERT_NE( p.get(), static_cast<int*>(NULL) );

			std::tr1::shared_ptr<int> p2 = p;
			p2.reset( alloc.allocate(1), Earlgrey::StlCustomDeletor<int>::Type(), Earlgrey::StlDefaultAllocator<int>::Type() );

			Earlgrey::StlGreedyAllocator<A> alloc2;
			A *a = alloc2.allocate(1);
			a->A::A();
			std::tr1::shared_ptr<A> pa = Earlgrey::create_shared_ptr<A>( a );
			ASSERT_EQ( pa->i, 0 );

			Earlgrey::StlGreedyAllocator<int*> alloc3;
			int** ipp = alloc3.allocate(1);
			*ipp = new int(3);
			std::tr1::shared_ptr<int*> ppi = Earlgrey::create_shared_ptr<int*>( ipp );
			delete *ppi;
		}
	}
}