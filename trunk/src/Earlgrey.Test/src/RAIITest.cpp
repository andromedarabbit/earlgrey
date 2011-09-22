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

		namespace
		{
			
			static bool HandleCalled = false;
			BOOL CloseHandleT(HANDLE handle)
			{				
				HandleCalled = true;
				return CloseHandle(handle);
			}

		}

		TEST(RAIITest, HandleT)
		{
			{
				ASSERT_FALSE(HandleCalled);

				// create Wait event
				HANDLE eventObj = CreateEvent(
					(LPSECURITY_ATTRIBUTES) 0,
					true,           // manual-reset
					false,          // initial state off
					(LPCTSTR) 0     // name
					);
				ASSERT_TRUE(eventObj != NULL);

				Earlgrey::handle_t handle(eventObj, &CloseHandleT);
				ASSERT_FALSE(HandleCalled);
			}

			ASSERT_TRUE(HandleCalled);

			// Earlgrey::handle_t schServiceHandle(schService, &CloseServiceHandle);

		}
	}
}