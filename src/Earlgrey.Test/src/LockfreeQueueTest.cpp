#include "stdafx.h"
#include "LockfreeQueue.hpp"

#include "Thread.h"

using namespace Earlgrey::Algorithm::Lockfree;

namespace Earlgrey
{
	namespace Test
	{
		TEST(LockfreeQueueTest, QueueTest)
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

		class LockFreeQueueThread : public SimpleThread
		{
		public:
			enum {
				SINGLE_PUSH_POP,
				ROLE_BASED
			};
			LockFreeQueueThread() : TestType(ROLE_BASED)
			{
			}

		public:
			int TestType;
			static volatile LONG s_fail_count;

		private:
			enum { MAX_VALUE = 10000 };

			DWORD Run()
			{
				bool push_role = InterlockedIncrement( &s_push ) % 2 == 0;
				while(IsRunning())
				{
					if (ROLE_BASED == TestType)
					{
						RoleBased(push_role);
					}
					else if (SINGLE_PUSH_POP == TestType)
					{
						push_role = !push_role;
						SinglePushPop(push_role);
					}
					
				}
				return 0;
			}

			void SinglePushPop(bool push_role)
			{
				LONG value = 0L;
				if (push_role)
				{
					s_container.Enqueue( value );
					return;
				}
				
				//s_container.MoveTail();
				if (!s_container.Dequeue( value ))
				{
					InterlockedIncrement( &s_fail_count );
				}
			}

			void RoleBased( bool push_role )
			{
				LONG value = 0L;
				if (push_role)
				{
					value = InterlockedIncrement( &s_value );
					s_container.Enqueue( value );
					return;
				}

				//s_container.MoveTail();

				if (!s_container.Dequeue( value ))
				{
					InterlockedIncrement( &s_fail_count );
				}

				DoSomething();
			}

			void DoSomething()
			{
				//InterlockedDecrement( &s_value );
			}

			static volatile LONG s_push;

			static volatile LONG s_value;
			static Algorithm::Lockfree::Queue<LONG> s_container;
			
		};

		volatile LONG LockFreeQueueThread::s_value;
		volatile LONG LockFreeQueueThread::s_push = 0;
		volatile LONG LockFreeQueueThread::s_fail_count = 0;
		Algorithm::Lockfree::Queue<LONG> LockFreeQueueThread::s_container;

		TEST(LockfreeQueueTest, ContentionTest)
		{
			enum { MAX_THREADS = 15 };
			LockFreeQueueThread QueueTestThread[MAX_THREADS];

			for (int i=0; i < MAX_THREADS; i++)
			{
				QueueTestThread[i].TestType = LockFreeQueueThread::SINGLE_PUSH_POP;
				ASSERT_EQ( QueueTestThread[i].Create(), TRUE );
			}

			Sleep( 5000 );

			for (int i=0; i < MAX_THREADS; i++)
			{
				QueueTestThread[i].Stop();
			}

			for (int i=0; i < MAX_THREADS; i++)
			{
				QueueTestThread[i].Join();
			}

			EXPECT_GT( 1, LockFreeQueueThread::s_fail_count );
		}
	}
}
