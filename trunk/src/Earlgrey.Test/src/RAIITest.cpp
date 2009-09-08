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

		/*
		struct LocalMemoryItem
		{
			static int LOCAL_MEMORY_DELETE_COUNT;

			~LocalMemoryItem() 
			{
				LOCAL_MEMORY_DELETE_COUNT++;
			}
		};
		int LocalMemoryItem::LOCAL_MEMORY_DELETE_COUNT = 0;

		TEST(RAIITest, LocalMemoryDeleter)
		{
			LocalMemoryItem* item = NULL;
			ASSERT_EQ(0, LocalMemoryItem::LOCAL_MEMORY_DELETE_COUNT);
			{
				HLOCAL handle = ::LocalAlloc(LPTR, sizeof(LocalMemoryItem));
				ASSERT_TRUE(handle != NULL);
				item = (LocalMemoryItem*)(::LocalLock(handle));
				ASSERT_TRUE(item != NULL);
				
				ASSERT_FALSE(IsBadReadPtr(item, sizeof(LocalMemoryItem)));
				// item->Item();

				shared_ptr<HLOCAL> bufferManagement(handle, &LocalFree );
								
			}
			ASSERT_TRUE(IsBadReadPtr(item, sizeof(LocalMemoryItem)));
			ASSERT_EQ(1, LocalMemoryItem::LOCAL_MEMORY_DELETE_COUNT);		
		}
		*/
	}
}