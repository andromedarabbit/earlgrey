#include "stdafx.h"
#include "shared_ptr_helper.h"

namespace Earlgrey
{
	namespace Test
	{
		using std::tr1::shared_ptr;

		TEST(SharedPtr, Basic)
		{
			shared_ptr<int> test = shared_ptr<int> (new int(3));
		}

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
			std::tr1::shared_ptr<int> p = Earlgrey::make_ptr<int>( alloc.allocate(1) );
			ASSERT_NE( p.get(), static_cast<int*>(NULL) );

			std::tr1::shared_ptr<int> p2 = p;
			Earlgrey::reset_ptr( p2, alloc.allocate(1) );

			Earlgrey::StlGreedyAllocator<A> alloc2;
			A *a = alloc2.allocate(1);
			a->A::A();
			std::tr1::shared_ptr<A> pa = Earlgrey::make_ptr<A>( a );
			ASSERT_EQ( pa->i, 0 );

			Earlgrey::StlGreedyAllocator<int*> alloc3;
			int** ipp = alloc3.allocate(1);
			*ipp = new int(3);
			std::tr1::shared_ptr<int*> ppi = Earlgrey::make_ptr<int*>( ipp );
			delete *ppi;
		}
	}
}