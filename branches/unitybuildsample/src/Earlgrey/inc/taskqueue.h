#pragma once
#include "LockfreeQueue.hpp"
#include "LockfreeStack.hpp"
#include "EarlgreyAssert.h"

#include <functional>

namespace Earlgrey {
	namespace Algorithm {
	namespace Lockfree {

		//! task queue 를 구현한다.
		/*!
			\param UnlockFType Unlock 메서드의 시그니처. Unlock 메서드를 호출할 때 넘겨지는 파라미터를 정의한다.
		*/
		template<class UnlockFType>
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

			typedef std::tr1::function<void()> TaskType;
			typedef std::tr1::function<UnlockFType> UnlockTaskType;

		private:
			class TaskHolder
			{
				static void DummyLock()
				{
				}
			public:
				enum {
					NORMAL,
					LOCK,
					LOCK_EVENT
				};

				explicit TaskHolder(const TaskType& task) : _task(task), _lock(NORMAL) {}

				explicit TaskHolder(const UnlockTaskType& unlockTask, int lock, LONG lockID = 0L) 
					: _task(&TaskHolder::DummyLock), _unlockTask(unlockTask), _lock(lock), _lockID(lockID) 
				{
				}

				explicit TaskHolder(const TaskType& task, const UnlockTaskType& unlockTask, int lock, LONG lockID = 0L) 
					: _task(task), _unlockTask(unlockTask), _lock(lock), _lockID(lockID) 
				{
				}

				void operator()()
				{
					_task();
				}

				void ExecuteUnlockTask()
				{
					_unlockTask();
				}

				template<typename T1>
				void ExecuteUnlockTask(T1 val1)
				{
					_unlockTask( val1 );
				}

				template<typename T1, typename T2>
				void ExecuteUnlockTask(T1 val1, T2 val2)
				{
					_unlockTask( val1, val2 );
				}

				template<typename T1, typename T2, typename T3>
				void ExecuteUnlockTask(T1 val1, T2 val2, T3 val3)
				{
					_unlockTask( val1, val2, val3 );
				}

				template<typename T1, typename T2, typename T3, typename T4>
				void ExecuteUnlockTask(T1 val1, T2 val2, T3 val3, T4 val4)
				{
					_unlockTask( val1, val2, val3, val4 );
				}

				template<typename T1, typename T2, typename T3, typename T4, typename T5>
				void ExecuteUnlockTask(T1 val1, T2 val2, T3 val3, T4 val4, T5 val5)
				{
					_unlockTask( val1, val2, val3, val4, val5 );
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
				TaskType _task;
				UnlockTaskType _unlockTask;
				int _lock;
				LONG _lockID;
			};

		public:

			//! Lock을 걸어 이후의 task가 unlock될 때까지 수행되지 못하도록 막는다.
			/*!
				\param task unlock될 때 호출 될 
			*/
			void Lock(const TaskType& task, const UnlockTaskType& unlockTask)
			{
				if (CAS( &_qlen, 0L, 1L ))
				{
					if (_lockState)
					{
						EARLGREY_ASSERT( _lockTask );

						// 이미 lock 상태이므로 lock task를 큐잉한다.
						_q.Enqueue( new TaskHolder( task, unlockTask, TaskHolder::LOCK ) );
						return;
					}

					EARLGREY_ASSERT( !_lockTask );

					task();

					_lockID *= 3;
					InterlockedExchange( &_lockState, _lockID );
					_lockTask = new TaskHolder( task, unlockTask, TaskHolder::LOCK, _lockState );

					InterlockedDecrement( &_qlen );	// 큐에는 넣지 않으므로 다시 0으로 설정한다.
				}
				else
				{
					_q.Enqueue( new TaskHolder( task, unlockTask, TaskHolder::LOCK ) );
					InterlockedIncrement( &_qlen );
				}
			}

			void Unlock()
			{
				PreUnlock();

				_lockTask->ExecuteUnlockTask();

				PostUnlock();
			}

			template<typename T1>
			void Unlock(T1 val1)
			{
				PreUnlock();

				_lockTask->ExecuteUnlockTask( val1 );

				PostUnlock();
			}

			template<typename T1, typename T2>
			void Unlock(T1 val1, T2 val2)
			{
				PreUnlock();

				_lockTask->ExecuteUnlockTask( val1, val2 );

				PostUnlock();
			}

			template<typename T1, typename T2, typename T3>
			void Unlock(T1 val1, T2 val2, T3 val3)
			{
				PreUnlock();

				_lockTask->ExecuteUnlockTask( val1, val2, val3 );

				PostUnlock();
			}

			template<typename T1, typename T2, typename T3, typename T4>
			void Unlock(T1 val1, T2 val2, T3 val3, T4 val4)
			{
				PreUnlock();

				_lockTask->ExecuteUnlockTask( val1, val2, val3, val4 );

				PostUnlock();
			}

			template<typename T1, typename T2, typename T3, typename T4, typename T5>
			void Unlock(T1 val1, T2 val2, T3 val3, T4 val4, T5)
			{
				PreUnlock();

				_lockTask->ExecuteUnlockTask( val1, val2, val3, val4, val5 );

				PostUnlock();
			}
			
		protected:
			//! lock the queue
			/*!
				This method MUST be called in a task.

				\param unlockTask is being invoked after unlock.
			*/
			void LockInTask(const UnlockTaskType& unlockTask)
			{
				if (_lockState)
				{
					EARLGREY_ASSERT( _lockTask );

					// 이미 lock 상태이므로 lock task를 큐잉한다. lockID 는 나중에 재설정되므로 설정할 필요가 없다.
					_q.Enqueue( new TaskHolder( unlockTask, TaskHolder::LOCK ) );
					InterlockedIncrement( &_qlen );
					return;
				}

				EARLGREY_ASSERT( _lockState == 0 );
				EARLGREY_ASSERT( _lockTask == NULL );
				EARLGREY_ASSERT( _qlen > 0 );

				// transit to LOCK state
				_lockID *= 3;
				InterlockedExchange( &_lockState, _lockID );
				_lockTask = new TaskHolder( unlockTask, TaskHolder::LOCK, _lockState );
			}

		private:
			void Post(const TaskType& task)
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
					{
						return;
					}

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
			void PreUnlock()
			{
				EARLGREY_ASSERT( _lockState );
				EARLGREY_ASSERT( _lockTask );
				EARLGREY_ASSERT( _lockTask->IsLockTask() );
				EARLGREY_ASSERT( _lockTask->GetLockID() == _lockState );
			}

			void PostUnlock()
			{
				delete _lockTask;
				_lockTask = NULL;

				InterlockedExchange( &_lockState, 0L );

				if (_qlen > 0)
				{
					ExecuteAllTasksInQueue();
				}	
			}

			void ExecuteAllTasksInQueue()
			{
				
				do {
					// A task can lock.
					if (_lockState)
					{
						EARLGREY_ASSERT( _lockTask );
						return;
					}

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

					(*taskHolder)();

					if (taskHolder->IsLockTask())
					{
						_lockID *= 3;
						InterlockedExchange( &_lockState, _lockID );
						taskHolder->SetLockID( _lockState );
						_lockTask = taskHolder;

						InterlockedDecrement( &_IsRunning );
						InterlockedDecrement( &_qlen );
						return;
					}
					
					delete taskHolder;
					InterlockedDecrement( &_IsRunning);
				} while(InterlockedDecrement( &_qlen ));
				
			}

		private:
			TaskHolder*	_lockTask;					//!< lock task
			Queue<TaskHolder*> _q;					//!< lockfree queue
			volatile LONG _qlen;					//!< the number of tasks in queue
			volatile LONG _waitQLen;				//!< the number of tasks in waiting queue
			volatile LONG _IsRunning;
			volatile LONG _lockState;				//!< default : 0
			volatile LONG _lockID;


		protected:
			template<typename T>
			void InvokeMethod(void (T::*func)())
			{
				TaskType f = std::tr1::bind(func, static_cast<T*>(this));
				Post( f );
			}

			template<typename T, typename T1>
			void InvokeMethod(void (T::*func)(T1), T1 t1)
			{
				TaskType f = std::tr1::bind(func, static_cast<T*>(this), t1);
				Post( f );
			}

			template<typename T, typename T1, typename T2>
			void InvokeMethod(void (T::*func)(T1,T2), T1 t1, T2 t2)
			{
				TaskType f = std::tr1::bind(func, static_cast<T*>(this), t1, t2);
				Post( f );
			}

			template<typename T, typename T1, typename T2, typename T3>
			void InvokeMethod(void (T::*func)(T1,T2,T3), T1 t1, T2 t2, T3 t3)
			{
				TaskType f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3);
				Post( f );
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4), T1 t1, T2 t2, T3 t3, T4 t4)
			{
				TaskType f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
			{
				TaskType f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4, t5);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
			{
				TaskType f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4, t5, t6);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6,T7), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
			{
				TaskType f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4, t5, t6, t7);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6,T7,T8), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
			{
				TaskType f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4, t5, t6, t7, t8);
				Post( f );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6,T7,T8,T9), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
			{
				TaskType f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4, t5, t6, t7, t8, t9);
				Post( f );
			}

			// 아래 함수는 bind()가 12개의 파라미터를 지원하지 않아서, 사용할 경우 컴파일 에러가 발생한다.
			/*template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T0>
			void InvokeMethod(void (T::*func)(T1,T2,T3,T4,T5,T6,T7,T8,T9,T0), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T0 t0)
			{
			TaskType f = std::tr1::bind(func, static_cast<T*>(this), t1, t2, t3, t4, t5, t6, t7, t8, t9, t0);
			Post( f );			
			}*/


		public:
			void Invoke(void (*func)())
			{
				TaskType f = std::tr1::bind(func);
				Post( f );
			}

			template<typename T1>
			void Invoke(void (*func)(T1), T1 t1)
			{
				TaskType f = std::tr1::bind(func, t1);
				Post( f );
			}

			template<typename T1, typename T2>
			void Invoke(void (*func)(T1,T2), T1 t1, T2 t2)
			{
				TaskType f = std::tr1::bind(func, t1, t2);
				Post( f );
			}

			template<typename T1, typename T2, typename T3>
			void Invoke(void (*func)(T1,T2,T3), T1 t1, T2 t2, T3 t3)
			{
				TaskType f = std::tr1::bind(func, t1, t2, t3);
				Post( f );			
			}

			template<typename T1, typename T2, typename T3, typename T4>
			void Invoke(void (*func)(T1,T2,T3,T4), T1 t1, T2 t2, T3 t3, T4 t4)
			{
				TaskType f = std::tr1::bind(func, t1, t2, t3, t4);
				Post( f );			
			}

			template<typename T1, typename T2, typename T3, typename T4, typename T5>
			void Invoke(void (*func)(T1,T2,T3,T4,T5), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
			{
				TaskType f = std::tr1::bind(func, t1, t2, t3, t4, t5);
				Post( f );			
			}

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
			void Invoke(void (*func)(T1,T2,T3,T4,T5,T6), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
			{
				TaskType f = std::tr1::bind(func, t1, t2, t3, t4, t5, t6);
				Post( f );			
			}

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
			void Invoke(void (*func)(T1,T2,T3,T4,T5,T6,T7), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
			{
				TaskType f = std::tr1::bind(func, t1, t2, t3, t4, t5, t6, t7);
				Post( f );			
			}

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
			void Invoke(void (*func)(T1,T2,T3,T4,T5,T6,T7,T8), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
			{
				TaskType f = std::tr1::bind(func, t1, t2, t3, t4, t5, t6, t7, t8);
				Post( f );			
			}

			template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
			void Invoke(void (*func)(T1,T2,T3,T4,T5,T6,T7,T8,T9), T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
			{
				TaskType f = std::tr1::bind(func, t1, t2, t3, t4, t5, t6, t7, t8, t9);
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