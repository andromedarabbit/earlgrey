#include "stdafx.h"
#include "taskqueue.h"
#include "SyncdMethod.hpp"
#include "EarlgreyAssert.h"

#include <functional>


using namespace Earlgrey::Algorithm::Lockfree;

namespace Earlgrey
{
	namespace Test
	{
		int intval_for_taskq = 0;

		// DECLARE_TASK1(SampleTask, int);
		class SampleTask : public Earlgrey::Algorithm::Lockfree::Task1<SampleTask, int>   
		{   
		public:   
			explicit SampleTask(int p1) : Earlgrey::Algorithm::Lockfree::Task1<SampleTask, int>( p1 )
			{
			}   

		private:   
			void UserDefinedFunction(int p1);   
		};

		void SampleTask::UserDefinedFunction(int val)
		{
			intval_for_taskq = val;
		}



		class NoParamTask : public Earlgrey::Algorithm::Lockfree::Task0<NoParamTask>
		{   
		public:
			explicit NoParamTask() 
			{
			}   

		private:   
			void UserDefinedFunction();
		};

		void NoParamTask::UserDefinedFunction()
		{
			intval_for_taskq = 100;
		}



		class TestTaskQueueClass : public TaskQueueClassBase
		{
			friend class SyncdMethod<TestTaskQueueClass, void()>;

		public:
			explicit TestTaskQueueClass() : _test(0) 
			{
			}
		
		public:   			
			
			void Set1 ()   
			{   
				/*
				if (Earlgrey::Algorithm::CAS( &_count, 0L, 1L ))
				{
					RawSet1();
					if (InterlockedDecrement( &_count ) == 0)
						return;   
				}
				else
				{   
					_q.Enqueue( new Set1_Queueable() );   
					if (InterlockedIncrement( &_count ) > 1)
						return;   
				}   
				ExecuteAllTasksInQueue();   
				*/

				std::tr1::function<void()> f 
					= std::tr1::bind(&TestTaskQueueClass::RawSet1, this);

				SyncdMethod<TestTaskQueueClass, void()> method(this, f);
				method.Execute();
			}

			void Set10 ()   
			{   
				std::tr1::function<void()> f 
					= std::tr1::bind(&TestTaskQueueClass::RawSet10, this);

				SyncdMethod<TestTaskQueueClass, void()> method(this, f);
				method.Execute();
			}


			void AddTwoValues (int a, int b)   
			{   
				std::tr1::function<void()> f 
					= std::tr1::bind(&TestTaskQueueClass::RawAddTwoValues, this, a, b);

				SyncdMethod<TestTaskQueueClass, void()> method(this, f);
				method.Execute();
			}

		private:   
			void RawSet1();
			void RawSet10();
			void RawAddTwoValues(int a, int b);

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

		TEST(TaskQueueTest, Simple)
		{
			// 아래 테스트는 단일 스레드에서 테스트하는 코드이므로 깨지면 안된다.
			SimpleTaskQueue taskq;
			taskq.Post(new SampleTask( 10 ));
			EXPECT_TRUE( intval_for_taskq == 10 );
			taskq.Post(new NoParamTask());
			EXPECT_TRUE( intval_for_taskq == 100 );

			TestTaskQueueClass* myQueueableClass = new TestTaskQueueClass();
			myQueueableClass->Set1();
			EXPECT_TRUE( myQueueableClass->_test == 1 );
			myQueueableClass->Set10();
			EXPECT_TRUE( myQueueableClass->_test == 10 );
			myQueueableClass->AddTwoValues( 10, 100 );
			EXPECT_TRUE( myQueueableClass->_test == 110 );
		}
	}
}