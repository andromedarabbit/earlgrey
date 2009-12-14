#include "stdafx.h"
#include "taskqueue.h"
#include "EarlgreyAssert.h"
#include "Application.h"
#include "Executor.h"

#include <functional>


using namespace Earlgrey::Algorithm::Lockfree;

namespace Earlgrey
{
	namespace Test
	{
		int intval_for_taskq = 0;

		class TestTaskQueueClass : public TaskQueue
		{
		public:
			explicit TestTaskQueueClass() : _test(0) 
			{
			}
		
		public:   			
			void Set1 ()   
			{   
				InvokeMethod( &TestTaskQueueClass::RawSet1, this );
			}

			void Set10 ()   
			{   
				InvokeMethod( &TestTaskQueueClass::RawSet10, this );
			}

			void AddTwoValues (int a, int b)   
			{   
				InvokeMethod( &TestTaskQueueClass::RawAddTwoValues, this, a, b );
			}

			void SumOfTen(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9)
			{
				InvokeMethod( &TestTaskQueueClass::RawSumOfTen, this, n1, n2, n3, n4, n5, n6, n7, n8, n9 );
			}

		private:   
			void RawSet1();
			void RawSet10();
			void RawAddTwoValues(int a, int b);
			void RawSumOfTen(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9);

		public:
			int _test;
		};

		void TestTaskQueueClass::RawSet1()
		{
			_test = 1;
		}

		void TestTaskQueueClass::RawSet10()
		{
			_test = 10;
		}

		void TestTaskQueueClass::RawAddTwoValues(int a, int b)
		{
			_test = a + b;
		}

		void TestTaskQueueClass::RawSumOfTen(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9)
		{
			_test = n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8 + n9;
		}

		void SetValue(int val)
		{
			intval_for_taskq = val;
		}

		void Set100()
		{
			intval_for_taskq = 100;
		}

		TaskQueueArray<TestTaskQueueClass> MyTaskQueueArray;

		TEST(TaskQueueTest, Simple)
		{
			// 아래 테스트는 단일 스레드에서 테스트하는 코드이므로 깨지면 안된다.
			TestTaskQueueClass* myQueueableClass = new TestTaskQueueClass();
			myQueueableClass->Set1();
			EXPECT_TRUE( myQueueableClass->_test == 1 );
			myQueueableClass->Set10();
			EXPECT_TRUE( myQueueableClass->_test == 10 );
			myQueueableClass->AddTwoValues( 10, 100 );
			EXPECT_TRUE( myQueueableClass->_test == 110 );
			myQueueableClass->SumOfTen( 1, 2, 3, 4, 5, 6, 7, 8, 9 );
			EXPECT_TRUE( myQueueableClass->_test == 45 );

			myQueueableClass->Invoke( SetValue, 10 );
			EXPECT_TRUE( intval_for_taskq == 10 );
			myQueueableClass->Invoke( Set100 );
			EXPECT_TRUE( intval_for_taskq == 100 );

			MyTaskQueueArray.Initialize( 100 );
			int index = MyTaskQueueArray.AllocateIndex();
			MyTaskQueueArray[index].Set1();
			EXPECT_TRUE( MyTaskQueueArray[index]._test == 1 );
		}


		class IncreaseTask : public TaskQueue
		{
		public:
			class TaskRunnable : public IRunnable {

			public:
				explicit TaskRunnable(IncreaseTask* task) : Task_(task)  {}
				virtual BOOL Init() { return TRUE;}
				virtual DWORD Run() {
					InterlockedIncrement(&Task_->AtomicCount_);
					
					Task_->InvokeMethod(&IncreaseTask::Increase, Task_);

					return 0;

				};
				virtual void Stop() {}
				virtual void OnStop() {}
			private:
				IncreaseTask* Task_;
			};
		public:
			explicit IncreaseTask(const int maxCount) 
				: AtomicCount_(0), Count_(0), EvenCount_(0), OddCount_(0), MaxCount_(maxCount)
			{
				Waiter_ = CreateEvent(NULL, TRUE, FALSE, NULL);
			}

			void Increase() {

				Count_++;

				if (Count_ % 100 == 0) {
					Sleep(10);
				}

				if (Count_%2) {
					EvenCount_++;
				}
				else {
					OddCount_++;
				}

				if (Count_ == MaxCount_ || MaxCount_ < 0) 
				{ 
					Done(); 
					return; 
				}
			}

			void Done() {
				SetEvent(Waiter_);

			}

			void WaitFor() {
				DWORD Success = WaitForSingleObject(Waiter_, INFINITE);
				EARLGREY_VERIFY(Success == WAIT_OBJECT_0); 
			}

			void Report() {
				printf("Count %d, Even %d, Odd %d\n", Count_, EvenCount_, OddCount_);
			}

		public:
			long AtomicCount_;
			int Count_;
			int EvenCount_;
			int OddCount_;
			int MaxCount_;
			HANDLE Waiter_;
		};


		TEST(TaskQueueTest, MultiThread) {

			const int maxCount = 10000;


			IncreaseTask *task = new IncreaseTask(maxCount);

			ASSERT_EQ(0, maxCount %2); // even count

			for (int i = 0; i < maxCount; i++) {
				IocpExecutorSingleton::Instance().Execute(RunnableBuilder::NewRunnable(new IncreaseTask::TaskRunnable(task)));
			}


			task->WaitFor();
			//task->Report();

			ASSERT_EQ(maxCount, task->AtomicCount_);
			ASSERT_EQ(maxCount, task->Count_);


		}
	}
}