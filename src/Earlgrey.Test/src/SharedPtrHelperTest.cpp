#include "stdafx.h"
#include "shared_ptr_helper.h"
#include "StlGreedyAllocator.hpp"
#include "RAII.h" // ArrayDeleter

namespace Earlgrey
{
	namespace Test
	{
		using std::tr1::shared_ptr;

		TEST(SharedPtrTest, AllocInt)
		{
			shared_ptr<int> test = shared_ptr<int> (new int(3));
		}

		TEST(SharedPtrTest, UsingArrayDeleter)
		{
			shared_ptr<double> test = shared_ptr<double> (
				new double[256]
				, ArrayDeleter<double>()
				);
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

			Earlgrey::StlDefaultAllocator<int>::Type alloc;
			shared_ptr<int> p = Earlgrey::make_ptr<int>( alloc.allocate(1) );
			ASSERT_NE( p.get(), static_cast<int*>(NULL) );


			shared_ptr<int> p2 = p;
			Earlgrey::reset_ptr( p2, alloc.allocate(1) );

			Earlgrey::StlDefaultAllocator<A>::Type alloc2;
			A *a = alloc2.allocate(1);
			a->A::A();
			shared_ptr<A> pa = Earlgrey::make_ptr<A>( a );
			ASSERT_EQ( pa->i, 0 );
		}

		TEST(SharedPtrHelperTest, AllocPrimitiveTypeArray)
		{	
			Earlgrey::StlDefaultAllocator<int*>::Type alloc3;
			int** ipp = alloc3.allocate(1);
			*ipp = new int(3);
			shared_ptr<int*> ppi = Earlgrey::make_ptr<int*>( ipp );
			delete *ppi;
		}
	}
}