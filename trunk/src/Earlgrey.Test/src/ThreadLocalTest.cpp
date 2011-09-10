#include "stdafx.h"
#include "ThreadLocal.hpp"
#include "Thread.h"
#include "ThreadRunnable.h"

namespace Earlgrey
{
	namespace Test
	{
		const int InitialValue_ = 5;

		class ThreadLocalTestRunnable : public ThreadRunnable
		{
		public:
			explicit ThreadLocalTestRunnable(int RunningSetValue) : RunningSetValue_(RunningSetValue) {}

			static ThreadLocalValue<int> TLSValue;

			int GetTLSValue() 
			{
				int value = RunningGetValue_;
				return value;
			}

		protected:
			virtual BOOL Init() { return TRUE; }
			virtual void Stop() {}
			virtual void Exit() {}


			virtual BOOL MeetsStopCondition() const
			{
				return FALSE;
			}

			virtual DWORD DoTask()
			{
				TLSValue = RunningSetValue_;

				RunningGetValue_ = TLSValue.Get();
				return EXIT_FAILURE;
			}

		private:
			int RunningGetValue_;
			int RunningSetValue_;
		};

		ThreadLocalValue<int> ThreadLocalTestRunnable::TLSValue(InitialValue_);


		class TLSTestThread : public SimpleThread
		{
		public:
			DWORD Run()
			{
				static ThreadLocalValue<int> testTLS;
				while (IsRunning())
				{
					testTLS.IsAllocated();
				}
				return 0;
			}
		};

		TEST(ThreadLocalTest, ContentionTest)
		{
			TLSTestThread testThread[10];
			for (int i=0; i < 10; i++)
			{
				EXPECT_TRUE2(testThread[i].Create());
			}

			Sleep( 1000 );

			for (int i=0; i < 10; i++)
			{
				testThread[i].Stop();
				testThread[i].Join();
			}
		}

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
			const int testValue = 100;

			EXPECT_FALSE(testValue == ThreadLocalTestRunnable::TLSValue.Get());

			ThreadLocalTestRunnable *runner = new ThreadLocalTestRunnable(testValue);
			
			std::tr1::shared_ptr<Thread> testThread = Thread::CreateThread(
				std::tr1::shared_ptr<ThreadRunnable>(static_cast<ThreadRunnable*>(runner))
				, "testRunnable" 
				, 0
				);
			EXPECT_TRUE( NULL != testThread );

			testThread->Join();

			int returnValue = runner->GetTLSValue();

			EXPECT_EQ(testValue, returnValue);


			EXPECT_FALSE(testValue == ThreadLocalTestRunnable::TLSValue.Get());

		}
	}
}