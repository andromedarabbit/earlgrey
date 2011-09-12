#include "stdafx.h"
#include "shared_ptr_helper.h"
#include "StlGreedyAllocator.hpp"
#include "RAII.h" // ArrayDeleter

#include "AsyncStream.h"

namespace Earlgrey
{
	namespace Test
	{
		using std::tr1::shared_ptr;

		class SimpleClass
		{
		public:
			explicit SimpleClass() 
				: i(0) 
			{

			}

			~SimpleClass() 
			{
				i = 100;
			}

			int i;
		};


		//TEST(SharedPtrHelperTest, MakePtr)
		//{
		//	std::tr1::shared_ptr<SimpleClass> obj = make_ptr(new SimpleClass());
		//	ASSERT_TRUE(obj != NULL);
		//}

		TEST(SharedPtrHelperTest, Basic)
		{
			Earlgrey::StlDefaultAllocator<int>::Type alloc;
			shared_ptr<int> p = Earlgrey::make_ptr<int>( alloc.allocate(1) );
			ASSERT_NE( p.get(), static_cast<int*>(NULL) );


			shared_ptr<int> p2 = p;
			Earlgrey::reset_ptr( p2, alloc.allocate(1) );

			Earlgrey::StlDefaultAllocator<SimpleClass>::Type alloc2;
			SimpleClass *obj = alloc2.allocate(1);
			obj->SimpleClass::SimpleClass();
			shared_ptr<SimpleClass> pa = Earlgrey::make_ptr<SimpleClass>( obj );
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