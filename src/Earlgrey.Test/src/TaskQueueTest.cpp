#include "stdafx.h"
#include "taskqueue.h"
#include "EarlgreyAssert.h"
#include "Application.h"
#include "Executor.h"
#include "PAL_Windows.h"

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
				InvokeMethod( &TestTaskQueueClass::RawSet1 );
			}

			void Set10 ()   
			{   
				InvokeMethod( &TestTaskQueueClass::RawSet10 );
			}

			void AddTwoValues (int a, int b)   
			{   
				InvokeMethod( &TestTaskQueueClass::RawAddTwoValues, a, b );
			}

			void SumOfTen(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9)
			{
				InvokeMethod( &TestTaskQueueClass::RawSumOfTen, n1, n2, n3, n4, n5, n6, n7, n8, n9 );
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

		TaskQueueArray<TestTaskQueueClass, int, 100> MyTaskQueueArray;

		TaskQueueHash<TestTaskQueueClass, int, 100> MyTaskQueueHash;

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

			MyTaskQueueArray[10].AddTwoValues( 20, 100 );
			EXPECT_EQ( MyTaskQueueArray[10]._test, 120 );
		}


		class IncreaseTask : public TaskQueue
		{
		public:
			class TaskRunnable : public IRunnable {

			public:
				explicit TaskRunnable(IncreaseTask* task) 
					: Task_(task)  
				{
				}
				
				virtual BOOL Init() { return TRUE; }
				
				virtual DWORD Run() 
				{
					InterlockedIncrement(&Task_->AtomicCount_);
					
					Task_->Increase();

					return EXIT_SUCCESS;
				};

				virtual void Stop() {}
				virtual void OnStop() {}

			private:
				IncreaseTask* Task_;
			};

		public:
			explicit IncreaseTask(const int maxCount) 
				: AtomicCount_(0)
				, Count_(0)
				, EvenCount_(0)
				, OddCount_(0)
				, MaxCount_(maxCount)
				, Waiter_(CreateEvent(NULL, TRUE, FALSE, NULL))
			{
				
			}

			void Increase()
			{
				InvokeMethod(&IncreaseTask::RawIncrease);
			}

			void Done() {
				SetEvent(Waiter_);
			}

			void WaitFor() {
				DWORD Success = WaitForSingleObject(Waiter_, INFINITE);
				EARLGREY_VERIFY(Success == WAIT_OBJECT_0); 
			}

// 			void Report() {
// 				printf("Count %d, Even %d, Odd %d\n", Count_, EvenCount_, OddCount_);
// 			}

			long AtomicCount() const 
			{
				return AtomicCount_;
			}
			
			int Count() const 
			{
				return Count_;
			}

		private:
			void RawIncrease() 
			{
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

		private:
			long AtomicCount_;
			int Count_;
			int EvenCount_;
			int OddCount_;
			int MaxCount_;
			HANDLE Waiter_;
		};


		TEST(TaskQueueTest, MultiThread) 
		{
			const int maxCount = 10000;

			// TODO: 이 객체 인스턴스는 누가 지우지?
			IncreaseTask * task = new IncreaseTask(maxCount);

			ASSERT_EQ(0, maxCount % 2); // even count

			for (int i = 0; i < maxCount; i++) 
			{
				IocpExecutorSingleton::Instance().Execute(
					RunnableBuilder::NewRunnable(new IncreaseTask::TaskRunnable(task))
					);
			}


			task->WaitFor();
			// task->Report();

			ASSERT_EQ(maxCount, task->AtomicCount());
			ASSERT_EQ(maxCount, task->Count());
		}


		class TaskQueueFixture : public ::testing::Test 
		{
		public:
			class MyQueue : public Earlgrey::Algorithm::Lockfree::TaskQueue
			{
			public:
				MyQueue() : _value(0L), _refCount(0L)
				{
					_waitEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
				}

				void AddRef()
				{
					AtomicInc( _refCount );
				}

				void Release()
				{
					if (0 == AtomicDec( _refCount ))
					{
						SetEvent( _waitEvent );
					}
				}

				void Add(DWORD d)
				{
					InvokeMethod( &MyQueue::RawAdd, d );
				}

				void LockSet0()
				{
					this->Lock( std::tr1::function<void()>(std::tr1::bind( &MyQueue::RawSet0, this )) );
				}

				DWORD GetValue() const
				{
					return _value;
				}

				HANDLE GetWaitHandle() const
				{
					return _waitEvent;
				}

			private:
				void RawAdd(DWORD d)
				{
					_value += d;
				}

				void RawSet0()
				{
					_value = 0;
				}

				DWORD _value;
				HANDLE _waitEvent;
				volatile LONG _refCount;
			};

		public:
			class TaskRunnable : public IRunnable 
			{
			public:
				explicit TaskRunnable(MyQueue* task) 
					: Task_(task)  
				{
				}

				virtual BOOL Init() { return TRUE; }

				virtual DWORD Run() 
				{
					for (int i=0; i < 10000; i++)
					{
						Task_->Add( 1L );
					}

					Task_->Release();

					return EXIT_SUCCESS;
				};

				virtual void Stop() {}
				virtual void OnStop() {}

			private:
				MyQueue* Task_;
			};


		public:
			MyQueue _taskQueue;
		};

		TEST_F( TaskQueueFixture, BasicTest )
		{
			_taskQueue.Add( 100L );
			EXPECT_EQ( _taskQueue.GetValue(), 100L );

			_taskQueue.Add( 10L );
			EXPECT_EQ( _taskQueue.GetValue(), 110L );

			_taskQueue.Add( 20L );
			EXPECT_EQ( _taskQueue.GetValue(), 130L );
		}


		TEST_F( TaskQueueFixture, MultiThreadTest )
		{
			for (int i = 0; i < 10; i++) 
			{
				_taskQueue.AddRef();
				IocpExecutorSingleton::Instance().Execute(
					RunnableBuilder::NewRunnable(new TaskRunnable(&_taskQueue))
					);
			}

			WaitForSingleObject( _taskQueue.GetWaitHandle(), INFINITE );

			EXPECT_EQ( _taskQueue.GetValue(), 100000L );
		}

		TEST_F( TaskQueueFixture, WaitTest )
		{
			for (int i = 0; i < 10; i++) 
			{
				if (i == 5)
				{
					_taskQueue.LockSet0();
				}
				_taskQueue.AddRef();
				IocpExecutorSingleton::Instance().Execute(
					RunnableBuilder::NewRunnable(new TaskRunnable(&_taskQueue))
					);
			}

			Sleep(1000);

			DWORD value = 100000L - _taskQueue.GetValue();
			
			_taskQueue.Unlock();

			WaitForSingleObject( _taskQueue.GetWaitHandle(), INFINITE );

			EXPECT_TRUE( _taskQueue.GetValue() == value );
		}
	}
}