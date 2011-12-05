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
			explicit LockFreeQueueThread() 
			{
				srand(GetTickCount());
			}

		public:
			static volatile LONG s_fail_count;
			static volatile LONG s_no_of_threads;

		private:
			DWORD Run()
			{
				bool push_role = InterlockedIncrement( &s_no_of_threads ) % 2 == 0;
				while(IsRunning())
				{
					push_role = !push_role;
					SinglePushPop(push_role);
					Sleep(rand() % 10);
				}
				return EXIT_SUCCESS;
			}

			void SinglePushPop(bool push_role)
			{
				LONG value = 0L;
				if (push_role)
				{
					s_container.Enqueue( value );
					return;
				}
				
				if (!s_container.Dequeue( value ))
				{
					InterlockedIncrement( &s_fail_count );
				}
			}

		private:			
			static Algorithm::Lockfree::Queue<LONG> s_container;
			
		};

		volatile LONG LockFreeQueueThread::s_no_of_threads = 0;
		// volatile LONG LockFreeQueueThread::s_value = 0;		
		volatile LONG LockFreeQueueThread::s_fail_count = 0;
		Algorithm::Lockfree::Queue<LONG> LockFreeQueueThread::s_container;

		TEST(LockfreeQueueTest, ContentionTest)
		{
			const int MAX_THREADS = 10;
			LockFreeQueueThread QueueTestThread[MAX_THREADS];

			for (int i = 0; i < MAX_THREADS; i++)
			{
				// QueueTestThread[i].TestType = LockFreeQueueThread::SINGLE_PUSH_POP;
				ASSERT_EQ( QueueTestThread[i].Create(), TRUE );
			}

			Sleep( 5000 );

			for (int i = 0; i < MAX_THREADS; i++)
			{
				QueueTestThread[i].Stop();
			}

			for (int i = 0; i < MAX_THREADS; i++)
			{
				QueueTestThread[i].Join();
			}

			ASSERT_EQ(MAX_THREADS, LockFreeQueueThread::s_no_of_threads);
			EXPECT_GT(LockFreeQueueThread::s_fail_count, 1l);
		}
	}
}
