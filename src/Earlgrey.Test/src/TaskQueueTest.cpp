#include "stdafx.h"
#include "taskqueue.h"
#include "EarlgreyAssert.h"

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
	}
}