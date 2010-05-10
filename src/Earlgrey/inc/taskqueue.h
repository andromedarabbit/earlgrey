#pragma once
#include "LockfreeQueue.hpp"
#include "LockfreeStack.hpp"
#include "EarlgreyAssert.h"

#include <functional>

namespace Earlgrey {
	namespace Algorithm {
	namespace Lockfree {

		class LockTask
		{

		};

		class UnlockTask
		{

		};

		class TaskQueue : private Uncopyable
		{
		public:
			explicit TaskQueue() 
				: _qlen(0)
				, _IsRunning(0)
				, _lockID(1)
				, _waitQLen(0)
				, _lockTask(NULL)
				, _lockState(0)
			{
			}

			virtual ~TaskQueue() 
			{
				// \todo erase all items from queue
			}

		private:
			class TaskHolder
			{
			public:
				enum {
					NORMAL,
					LOCK,
					LOCK_EVENT
				};

				explicit TaskHolder(const std::tr1::function<void()>& task) : _task(task), _lock(NORMAL) {}

				explicit TaskHolder(const std::tr1::function<void()>& task, int lock, LONG lockID = 0L) : _task(task), _lock(lock), _lockID(lockID) {}

				void operator()()
				{
					_task();
				}

				bool IsLockTask() const
				{
					return _lock == LOCK;
				}

				void SetLockID(LONG lockID)
				{
					_lockID = lockID;
				}

				LONG GetLockID() const
				{
					return _lockID;
				}

			private:
				std::tr1::function<void()> _task;
				int _lock;
				LONG _lockID;
			};

		public:

			//! Lock을 걸어 이후의 task가 unlock될 때까지 수행되지 못하도록 막는다.
			/*!
				\param task unlock될 때 호출 될 
			*/
			void Lock(const std::tr1::function<void()>& task)
			{
				if (CAS( &_qlen, 0L, 1L ))
				{
					if (_lockState)
					{
						EARLGREY_ASSERT( _lockTask );

						// 이미 lock 상태인데 큐가 빈 상태이므로 lock task를 큐잉한다.
						_q.Enqueue( new TaskHolder( task, TaskHolder::LOCK ) );
						return;
					}

					EARLGREY_ASSERT( !_lockTask );

					_lockID *= 3;
					InterlockedExchange( &_lockState, _lockID );
					_lockTask = new TaskHolder( task, TaskHolder::LOCK, _lockState );

					InterlockedDecrement( &_qlen );	// 큐에는 넣지 않으므로 다시 0으로 설정한다.
				}
				else
				{
					_q.Enqueue( new TaskHolder( task, TaskHolder::LOCK ) );
					InterlockedIncrement( &_qlen );
				}
			}

			void Unlock()
			{
				EARLGREY_ASSERT( _lockState );
				EARLGREY_ASSERT( _lockTask );
				EARLGREY_ASSERT( _lockTask->IsLockTask() );
				EARLGREY_ASSERT( _lockTask->GetLockID() == _lockState );

				(*_lockTask)();

				delete _lockTask;
				_lockTask = NULL;

				InterlockedExchange( &_lockState, 0L );

				if (_qlen > 0)
				{
					ExecuteAllTasksInQueue();
				}				
			}

		private:
			void Post(const std::tr1::function<void()>& task)
			{
				if (_lockState)
				{
					// lock인 상태에서는 항상 큐잉만 한다.
					_q.Enqueue( new TaskHolder( task ) );
					InterlockedIncrement( &_qlen );
					return;
				}

				// Is there any thread in process?
				if (CAS( &_qlen, 0L, 1L ))
				{
					task();

					// there is no thread in process, then return.
					if (InterlockedDecrement( &_qlen ) == 0)
						return;

					// if not, execute all task in queue.
					// other threads MUST have enqueued their tasks.
				}
				else
				{
					_q.Enqueue( new TaskHolder( task ) );

					// if there is a thread in process, don't do anything.
					if (InterlockedIncrement( &_qlen ) > 1)
						return;

					// if not, execute the task that this thread has just enqueued.
					// Because there is no thread that can execute the task.
				}
				
				ExecuteAllTasksInQueue();
			}

		private:
			void ExecuteAllTasksInQueue()
			{
				
				do {
					InterlockedIncrement( &_IsRunning);
					EARLGREY_ASSERT( _qlen > 0 );
					TaskHolder* taskHolder = NULL;
					EARLGREY_ASSERT(CAS( &_IsRunning, 1L, 1L ) == 1L);
					for (;;) {
						taskHolder = NULL;

						// 이걸 호출하지 않으면 _qlen이 1인데도 Dequeue() 가 false 를 리턴하여 루프를 빠져나올 수 있다.
						// 그렇게 되면 이후의 Post() 는 task를 큐잉만 한다.
						_q.MoveTail();
						bool hasTask = _q.Dequeue( taskHolder );
						if (hasTask) break;
					}
					EARLGREY_ASSERT(taskHolder != NULL);

					if (taskHolder->IsLockTask())
					{
						_lockID *= 3;
						InterlockedExchange( &_lockState, _lockID );
						taskHolder->SetLockID( _lockState );
						_lockTask = taskHolder;

						InterlockedDecrement( &_qlen );
						return;
					}
					
					(*taskHolder)();
					delete taskHolder;
					InterlockedDecrement( &_IsRunning);
				} while(InterlockedDecrement( &_qlen ));
				
			}

		private:
			TaskHolder*	_lockTask;					//!< lock task
			Queue<TaskQueue::TaskHolder*> _q;		//!< lockfree queue
			volatile LONG _qlen;					//!< the number of tasks in queue
			volatile LONG _waitQLen;				//!< the number of tasks in waiting queue
			volatile LONG _IsRunning;
			volatile LONG _lockState;				//!< default : 0
			volatile LONG _lockID;


		protected:
			template<typename T>
			void InvokeMethod(void (T::*func)())
			{
				std::tr1::function<void()> f = std::tr1::bind(func, static_cast<T*>(this));
				Post( f );
			}

			template<typename T, typename T1>
			void InvokeMethod(void (T::*func)(T1), T1 t1)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, static_cast<T*>(this), t1);
				Post( f );
			}

			template<typename T, typename T1, typename T2>
			void InvokeMethod(void (T::*func)(T1,T2), T1 t1, T2 t2)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, static_cast<T*>(this), t1, t2);
				Post( f );
			}

			template<typename T, typename T1, typename T2, typename T3>
			void InvokeMethod(void (T::*func)(T1,T2,T3), T1 t1, T2 t2, T3 t3)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3);
				Post( f );
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4), T1 t1, T2 t2, T3 t3, T4 t4)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4, t5);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4, t5, t6);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6,T7), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4, t5, t6, t7);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6,T7,T8), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4, t5, t6, t7, t8);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6,T7,T8,T9), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4, t5, t6, t7, t8, t9);
				Post( f );
			}

			// 아래 함수는 bind()가 12개의 파라미터를 지원하지 않아서, 사용할 경우 컴파일 에러가 발생한다.
			/*template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T0>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6,T7,T8,T9,T0), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T0 t0)
			{
			std::tr1::function<void()> f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4, t5, t6, t7, t8, t9, t0);
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


		struct SimpleHashFunction
		{
			template<typename T>
			static T Hash(T value, T bucketSize)
			{
				return value % bucketSize;
			}
		};


		template<class T, typename IndexType, IndexType BucketSize, class HashFn = SimpleHashFunction>
		class TaskQueueHash
		{
		public:
			typedef T TaskQueueType;

			TaskQueueType& operator[](IndexType Index) 
			{
				return _TaskQueue[HashFn::Hash( Index, BucketSize )];
			}

		private:
			TaskQueueType _TaskQueue[BucketSize];
		};

		template<typename T, typename IndexType, IndexType Size>
		class TaskQueueArray
		{
		public:
			typedef T TaskQueueType;
			enum { MAX = Size };

			TaskQueueType& operator[](IndexType Index)
			{ 
				if (Index >= Size)
				{
					throw std::overflow_error("out of range");
				}
				return _TaskQueueArray[Index]; 
			}

		private:
			TaskQueueType _TaskQueueArray[Size];
		};

	} // end of Lockfree namespace
	} // end of Thread namespace
}