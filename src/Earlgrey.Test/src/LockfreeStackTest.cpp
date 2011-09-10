#include "stdafx.h"
#include "LockfreeStack.hpp"
#include "LockfreeQueue.hpp"

#include "Thread.h"

using namespace Earlgrey::Algorithm::Lockfree;

namespace Earlgrey
{
	namespace Test
	{

		class LockFreeStackThread : public SimpleThread
		{
		public:
			LockFreeStackThread() : m_push(true)
			{
			}

		private:
			enum { MAX_VALUE = 100000 };

			DWORD Run()
			{
				while(IsRunning())
				{
					LONG value = 0L;

					if (m_push)
					{
						value = InterlockedIncrement( &s_value );
						s_container.Push( value );

						if (value > MAX_VALUE)
						{
							m_push = false;
						}
						continue;
					}

					if (!s_container.Pop( value ))
					{
						break;
					}
				}
				return 0;
			}

			bool m_push;

			static volatile LONG s_value;
			static Algorithm::Lockfree::Stack<LONG> s_container;
		};

		volatile LONG LockFreeStackThread::s_value;
		Algorithm::Lockfree::Stack<LONG> LockFreeStackThread::s_container;

		TEST(LockFreeTest, ContentionTest)
		{
			enum { MAX_THREADS = 2 };
			LockFreeStackThread StackTestThread[MAX_THREADS];

			for (int i=0; i < MAX_THREADS; i++)
			{
				ASSERT_EQ( StackTestThread[i].Create(), TRUE );
			}

			Sleep( 1000 );

			for (int i=0; i < MAX_THREADS; i++)
			{
				StackTestThread[i].Stop();
			}

			for (int i=0; i < MAX_THREADS; i++)
			{
				StackTestThread[i].Join();
			}
		}

		TEST(LockFreeTest, Pointer) 
		{
			int* test = new int(1);

			

			Pointer<int> pointer1;

			int pointerSize = sizeof(Pointer<int>);

			ASSERT_EQ(8, pointerSize);

			pointer1.p(test);

			Pointer<int> pointer2;

			pointer2 = pointer1;
	
			ASSERT_EQ(pointer1.p(), pointer2.p());
			ASSERT_EQ(pointer1.Count(), pointer2.Count());
			ASSERT_NE(pointer1.val64, pointer2.val64);

			pointer1.Count(pointer1.Count() + 1);
			ASSERT_EQ(pointer1.p(), pointer2.p());
			ASSERT_TRUE(pointer1.Count() > pointer2.Count());
			ASSERT_NE(pointer1.val64, pointer2.val64);

		}

		TEST(LockFreeTest, StackTest)
		{
			Stack<int> stack;
			int value = 0;

			EXPECT_FALSE(stack.Pop( value ));

			stack.Push(10);
			stack.Push(9);

			EXPECT_TRUE(stack.Pop( value ));
			EXPECT_TRUE(value == 9);
			EXPECT_TRUE(stack.Pop( value ));
			EXPECT_TRUE(value == 10);
			EXPECT_FALSE(stack.Pop( value ));
		}

		TEST(LockFreeTest, QueueTest)
		{
			Queue<int> queue;
			int value = 0;

			EXPECT_FALSE(queue.Dequeue( value ));

			queue.Enqueue(10);
			queue.Enqueue(1);

			EXPECT_TRUE(queue.Dequeue( value ));
			EXPECT_TRUE(value == 10);
			EXPECT_TRUE(queue.Dequeue( value ));
			EXPECT_TRUE(value == 1);
			EXPECT_FALSE(queue.Dequeue( value ));
		}
	}
}
