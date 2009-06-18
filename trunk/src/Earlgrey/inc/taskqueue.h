#pragma once
#include "LockfreeQueue.hpp"

namespace Earlgrey {
	namespace Algorithm {
	namespace Lockfree {

		class TaskQueue : private Uncopyable
		{
		public:
			explicit TaskQueue() : _count(0) {}
			virtual ~TaskQueue() 
			{
				// \todo erase all items from queue
			}

		protected:
			class TaskHolder
			{
			public:
				explicit TaskHolder(const std::tr1::function<void()>& task) : _task(task) {}
				void operator()()
				{
					_task();
				}
			private:
				std::tr1::function<void()> _task;
			};

		protected:
			void Post(const std::tr1::function<void()>& task)
			{
				// Is there any thread in process?
				if (CAS( &_count, 0L, 1L ))
				{
					task();

					// there is no thread in process, then return.
					if (InterlockedDecrement( &_count ) == 0)
						return;

					// if not, execute all task in queue.
					// other threads MUST have enqueued their tasks.
				}
				else
				{
					_q.Enqueue( new TaskHolder( task ) );

					// if there is a thread in process, don't do anything.
					if (InterlockedIncrement( &_count ) > 1)
						return;

					// if not, execute the task that this thread has just enqueued.
					// Because there is no thread that can execute the task.
				}
				
				ExecuteAllTasksInQueue();
			}

		protected:
			void ExecuteAllTasksInQueue()
			{
				_ASSERTE( _count > 0 );
				TaskHolder* taskHolder = NULL;
				do {
					bool isEmpty = _q.Dequeue( taskHolder );
					_ASSERTE( !isEmpty );	// if empty, it's critical error, because _count is not matched.
					if (!isEmpty)
					{
						(*taskHolder)();
						delete taskHolder;
					}
				} while(InterlockedDecrement( &_count ));
			}

		protected:
			Queue<TaskQueue::TaskHolder*> _q;		//!< lockfree queue
			volatile LONG _count;		//!< the number of tasks in queue

		protected:
			template<typename T>
			void InvokeMethod(void (T::*func)(), T* subthis)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis);
				Post( f );
			}

			template<typename T, typename T1>
			void InvokeMethod(void (T::*func)(T1), T* subthis, T1 t1)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1);
				Post( f );
			}

			template<typename T, typename T1, typename T2>
			void InvokeMethod(void (T::*func)(T1,T2), T* subthis, T1 t1, T2 t2)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2);
				Post( f );
			}

			template<typename T, typename T1, typename T2, typename T3>
			void InvokeMethod(void (T::*func)(T1,T2,T3), T* subthis, T1 t1, T2 t2, T3 t3)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3);
				Post( f );
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4, t5);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4, t5, t6);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6,T7), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4, t5, t6, t7);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6,T7,T8), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4, t5, t6, t7, t8);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6,T7,T8,T9), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4, t5, t6, t7, t8, t9);
				Post( f );
			}

			// 아래 함수는 bind()가 12개의 파라미터를 지원하지 않아서, 사용할 경우 컴파일 에러가 발생한다.
			/*template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T0>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6,T7,T8,T9,T0), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T0 t0)
			{
			std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4, t5, t6, t7, t8, t9, t0);
			Post( f );			
			}*/


		public:
			void Invoke(void (*func)())
			{
				std::tr1::function<void()> f = std::tr1::bind(func);
				Post( f );
			}

			template<typename T1>
			void Invoke(void (*func)(T1), T1 t1)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, t1);
				Post( f );
			}

			template<typename T1, typename T2>
			void Invoke(void (*func)(T1,T2), T1 t1, T2 t2)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, t1, t2);
				Post( f );
			}

			template<typename T1, typename T2, typename T3>
			void Invoke(void (*func)(T1,T2,T3), T1 t1, T2 t2, T3 t3)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, t1, t2, t3);
				Post( f );			
			}

			template<typename T1, typename T2, typename T3, typename T4>
			void Invoke(void (*func)(T1,T2,T3,T4), T1 t1, T2 t2, T3 t3, T4 t4)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, t1, t2, t3, t4);
				Post( f );			
			}

			template<typename T1, typename T2, typename T3, typename T4, typename T5>
			void Invoke(void (*func)(T1,T2,T3,T4,T5), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, t1, t2, t3, t4, t5);
				Post( f );			
			}

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
			void Invoke(void (*func)(T1,T2,T3,T4,T5,T6), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, t1, t2, t3, t4, t5, t6);
				Post( f );			
			}

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
			void Invoke(void (*func)(T1,T2,T3,T4,T5,T6,T7), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, t1, t2, t3, t4, t5, t6, t7);
				Post( f );			
			}

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
			void Invoke(void (*func)(T1,T2,T3,T4,T5,T6,T7,T8), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, t1, t2, t3, t4, t5, t6, t7, t8);
				Post( f );			
			}

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
			void Invoke(void (*func)(T1,T2,T3,T4,T5,T6,T7,T8,T9), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, t1, t2, t3, t4, t5, t6, t7, t8, t9);
				Post( f );
			}
		};

		template<typename T>
		class TaskQueueArray
		{
			
		};

	} // end of Lockfree namespace
	} // end of Thread namespace
}