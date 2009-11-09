#include "stdafx.h"
#include "ThreadLocal.hpp"
#include "Thread.h"

namespace Earlgrey
{
	namespace Test
	{
		const int InitialValue_ = 5;

		class ThreadLocalTestRunnable : public IRunnable
		{
		public:


			ThreadLocalValue<int> TLSValue();


			BOOL Init() { return TRUE; }
			DWORD Run() 
			{
				return 0;
			}

			void Stop() {}
			void Exit() {}
		};

		TEST(ThreadLocalTest, IntTest)
		{
			const int integerValue = 5;

			ThreadLocalValue<int> tlsInteger(integerValue);
			int tlsValue1 = tlsInteger.Get();

			ASSERT_EQ(integerValue, tlsValue1);

			int* tlsValue2 = &tlsValue1;
			// int* tlsValue2 = tlsInteger;;

			ASSERT_EQ(integerValue, *tlsValue2);
		}

		TEST(ThreadLocalTest, IntPtrTest)
		{
			int integerValue = 3;

			ThreadLocalValue<int*> tlsInteger(&integerValue);
			int* tlsValue1 = tlsInteger.Get();

			ASSERT_EQ(integerValue, *tlsValue1);

			int tlsValue2 = *tlsInteger;
			
			ASSERT_EQ(integerValue, tlsValue2);
		}

		TEST(ThreadLocalTest, EachThreadTest)
		{
			std::tr1::shared_ptr<Thread> testThread = Thread::CreateThread(
				std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new ThreadLocalTestRunnable())), "testRunnable" );
			EXPECT_TRUE( NULL != testThread );

			testThread->Join();

		}
	}
}