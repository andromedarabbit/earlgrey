#include "stdafx.h"
#include "SharedObject.h"

namespace Earlgrey {
	namespace Test {
		class A {
		public:
			int i;
		};

		TEST(SharedPtrTest, BasicTest)
		{
			SharedPtr<A> ptr( new A() );
			SharedPtr<A> ptr2 = ptr;
			ptr.Reset( NULL );

			ASSERT_EQ(ptr.Get(), static_cast<const A*>(NULL));
			ASSERT_NE(ptr2.Get(), static_cast<const A*>(NULL));
			ASSERT_EQ(ptr2.IsUnique(), true);

			ptr2->i = 0;

			SharedPtr<A> ptr3;
			ASSERT_EQ(ptr3.IsUnique(), false);
			ASSERT_EQ(ptr3 < ptr2, true);

			SharedPtr<A> ptr4( new A() );
			SharedPtr<A> ptr5 = ptr4;

			ptr4.Swap( ptr2 );

			ASSERT_EQ( ptr2, ptr5 );
			ASSERT_NE( ptr4, ptr5 );
			ASSERT_EQ( ptr5.IsUnique(), false );
		}
	}
}