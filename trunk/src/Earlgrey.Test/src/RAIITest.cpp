#include "stdafx.h"

#include "RAII.h"
#include <memory>

namespace Earlgrey
{
	namespace Test
	{
		using namespace std;
		using namespace std::tr1;

		struct Item
		{
			static int COUNT;

			~Item() 
			{
				COUNT++;
			}
		};
		int Item::COUNT = 0;

		TEST(RAIITest, ArrayDeleter)
		{
			ASSERT_EQ(0, Item::COUNT);
			{
				shared_ptr<Item> items(new Item [256], ArrayDeleter<Item>() );
			}
			ASSERT_EQ(256, Item::COUNT);			
		}
	}
}