#include "stdafx.h"

#include "RAII.h"
#include <memory>

namespace Earlgrey
{
	namespace Test
	{
		using namespace std;
		using namespace std::tr1;

		struct ArrayItem
		{
			static int DELETE_COUNT;
			
			~ArrayItem() 
			{
				DELETE_COUNT++;
			}
		};
		int ArrayItem::DELETE_COUNT = 0;
	
		TEST(RAIITest, ArrayDeleter)
		{
			ASSERT_EQ(0, ArrayItem::DELETE_COUNT);
			{
				shared_ptr<ArrayItem> items(new ArrayItem [256], ArrayDeleter<ArrayItem>() );
			}
			ASSERT_EQ(256, ArrayItem::DELETE_COUNT);			
		}
	}
}