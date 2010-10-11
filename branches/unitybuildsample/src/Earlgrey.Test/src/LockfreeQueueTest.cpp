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
			LockFreeQueueThread() : m_push(true)
			{
			}

		private:
			enum { MAX_VALUE = 10000 };

			DWORD Run()
			{
				while(IsRunning())
				{
					LONG value = 0L;

					if (m_push)
					{
						value = InterlockedIncrement( &s_value );
						s_container.Enqueue( value );

						if (value > MAX_VALUE)
						{
							m_push = false;
						}
						continue;
					}

					s_container.MoveTail();

					if (!s_container.Dequeue( value ))
					{
						break;
					}

					DoSomething();
				}
				return 0;
			}

			void DoSomething()
			{
				InterlockedDecrement( &s_value );
			}

			bool m_push;

			static volatile LONG s_value;
			static Algorithm::Lockfree::Queue<LONG> s_container;
		};

		volatile LONG LockFreeQueueThread::s_value;
		Algorithm::Lockfree::Queue<LONG> LockFreeQueueThread::s_container;

		TEST(LockfreeQueueTest, ContentionTest)
		{
			enum { MAX_THREADS = 2 };
			LockFreeQueueThread QueueTestThread[MAX_THREADS];

			for (int i=0; i < MAX_THREADS; i++)
			{
				ASSERT_EQ( QueueTestThread[i].Create(), TRUE );
			}

			Sleep( 1000 );

			for (int i=0; i < MAX_THREADS; i++)
			{
				QueueTestThread[i].Stop();
			}

			for (int i=0; i < MAX_THREADS; i++)
			{
				QueueTestThread[i].Join();
			}
		}
	}
}
