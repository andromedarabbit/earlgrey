#pragma once
#include "LockfreeQueue.hpp"
#include "SyncdMethod.hpp"

namespace Earlgrey {
	namespace Algorithm {
	namespace Lockfree {

		class ITaskBase
		{
		public:
			virtual void Run() = 0;
			virtual void Destory() = 0;
		};

		template<typename T>
		class Task0 : public ITaskBase
		{
		public:
			void Run()
			{
				UserDefinedFunction();
				Destory();
			}

		protected:
			void Destory()
			{
				delete (T*)this;
			}

			virtual void UserDefinedFunction() = 0;
		};

		template<typename T, typename P1>
		class Task1 : public ITaskBase
		{
		public:
			explicit Task1(P1 p1) : _p1(p1) {}

			void Run()
			{
				UserDefinedFunction( _p1 );
				Destory();
			}

		protected:
			void Destory()
			{
				delete (T*)this;
			}

			virtual void UserDefinedFunction(P1 p1) = 0;

		private:
			P1 _p1;
		};


		template<typename TaskType>
		class TaskQueueBase : private Uncopyable
		{
		public:
			explicit TaskQueueBase() : _count(0) 
			{
			}

			virtual ~TaskQueueBase() 
			{
				// \todo erase all items from queue
			}

			void Post(TaskType* task)
			{
				// Is there any thread in process?
				if (CAS( &_count, 0L, 1L ))
				{
					ExecuteTask( task );

					// there is no thread in process, then return.
					if (InterlockedDecrement( &_count ) == 0)
						return;

					// if not, execute all task in queue.
					// other threads MUST have enqueued their tasks.
				}
				else
				{
					_q.Enqueue( task );

					// if there is a thread in process, don't do anything.
					if (InterlockedIncrement( &_count ) > 1)
						return;

					// if not, execute the task that this thread has just enqueued.
					// Because there is no thread that can execute the task.
				}
				
				ExecuteAllTasksInQueue();
			}

		protected:
			//! template method for executing a task
			virtual void ExecuteTask(TaskType*) = 0;

			void ExecuteAllTasksInQueue()
			{
				_ASSERTE( _count > 0 );
				TaskType* queuedTask = NULL;
				do {
					bool isEmpty = _q.Dequeue( queuedTask );
					_ASSERTE( !isEmpty );	// if empty, it's critical error, because _count is not matched.
					if (!isEmpty)
					{
						ExecuteTask( queuedTask );
					}
				} while(InterlockedDecrement( &_count ));
			}

		protected:
			Queue<TaskType*> _q;		//!< lockfree queue
			volatile LONG _count;		//!< the number of tasks in queue
		};

		class SimpleTaskQueue : public TaskQueueBase<ITaskBase>
		{
		private:
			void ExecuteTask(ITaskBase* task)
			{
				task->Run();
			}
		};

		class TaskQueueClassBase : public TaskQueueBase<IQueueableMethod>
		{
			friend class SyncdMethod<TaskQueueClassBase, void()>;
		public:
			explicit TaskQueueClassBase() {}
			virtual ~TaskQueueClassBase() {}
		private:
			void ExecuteTask(IQueueableMethod* method)
			{
				method->Execute();
			}

			template<typename T>
			void _execute(std::tr1::function<void()>& f, T* subthis)
			{
				SyncdMethod<T, void()> method(subthis, f);
				method.Execute();
			}
			
		protected:
			template<typename T>
			void call(void (T::*func)(), T* subthis)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis);
				_execute<T>( f, subthis );
			}

			template<typename T, typename T1>
			void call(void (T::*func)(T1), T* subthis, T1 t1)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1);
				_execute<T>( f, subthis );
			}

			template<typename T, typename T1, typename T2>
			void call(void (T::*func)(T1,T2), T* subthis, T1 t1, T2 t2)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2);
				_execute<T>( f, subthis );			
			}

			template<typename T, typename T1, typename T2, typename T3>
			void call(void (T::*func)(T1,T2,T3), T* subthis, T1 t1, T2 t2, T3 t3)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3);
				_execute<T>( f, subthis );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4>
			void call(void (T::*func)(T1,T2,T3,T4), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4);
				_execute<T>( f, subthis );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
			void call(void (T::*func)(T1,T2,T3,T4,T5), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4, t5);
				_execute<T>( f, subthis );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
			void call(void (T::*func)(T1,T2,T3,T4,T5,T6), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4, t5, t6);
				_execute<T>( f, subthis );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
			void call(void (T::*func)(T1,T2,T3,T4,T5,T6,T7), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4, t5, t6, t7);
				_execute<T>( f, subthis );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
			void call(void (T::*func)(T1,T2,T3,T4,T5,T6,T7,T8), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4, t5, t6, t7, t8);
				_execute<T>( f, subthis );			
			}

			template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
			void call(void (T::*func)(T1,T2,T3,T4,T5,T6,T7,T8,T9), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4, t5, t6, t7, t8, t9);
				_execute<T>( f, subthis );			
			}

			// 아래 함수는 bind()가 12개의 파라미터를 지원하지 않아서, 사용할 경우 컴파일 에러가 발생한다.
			/*template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T0>
			void call(void (T::*func)(T1,T2,T3,T4,T5,T6,T7,T8,T9,T0), T* subthis, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T0 t0)
			{
				std::tr1::function<void()> f = std::tr1::bind(func, subthis, t1, t2, t3, t4, t5, t6, t7, t8, t9, t0);
				_execute<T>( f, subthis );			
			}*/
		};


	} // end of Lockfree namespace
	} // end of Thread namespace
}