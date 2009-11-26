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

			explicit ThreadLocalTestRunnable(int RunningSetValue) : RunningSetValue_(RunningSetValue) {}



			static ThreadLocalValue<int> TLSValue;

			int GetTLSValue() 
			{

				int value = RunningGetValue_;

				return value;
			}


			BOOL Init() { return TRUE; }
			DWORD Run() 
			{
				TLSValue = RunningSetValue_;

				RunningGetValue_ = TLSValue.Get();
				return 0;
			}

			void Stop() {}
			void Exit() {}

		private:
			int RunningGetValue_;
			int RunningSetValue_;
		};

		ThreadLocalValue<int> ThreadLocalTestRunnable::TLSValue(InitialValue_);

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
				std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(runner)), "testRunnable" );
			EXPECT_TRUE( NULL != testThread );

			testThread->Join();

			int returnValue = runner->GetTLSValue();

			EXPECT_EQ(testValue, returnValue);


			EXPECT_FALSE(testValue == ThreadLocalTestRunnable::TLSValue.Get());

		}
	}
}