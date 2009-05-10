#pragma once
#include "LockfreeQueue.hpp"

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
					Execute( task );

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
			virtual void Execute(TaskType*) = 0;

			void ExecuteAllTasksInQueue()
			{
				_ASSERTE( _count > 0 );
				TaskType* queuedTask = NULL;
				do {
					bool isEmpty = _q.Dequeue( queuedTask );
					_ASSERTE( !isEmpty );	// if empty, it's critical error, because _count is not matched.
					if (!isEmpty)
					{
						Execute( queuedTask );
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
			void Execute(ITaskBase* task)
			{
				task->Run();
			}
		};

		class IQueueableMethod 
		{
		public:
			//! This is a hook method will be called when a task is dequeued.
			// virtual void Execute(class TaskQueueClassBase* taskQueue) = 0;
			virtual void Execute() = 0;
		};

		class TaskQueueClassBase : public TaskQueueBase<IQueueableMethod>
		{
		public:
			explicit TaskQueueClassBase() {}
			virtual ~TaskQueueClassBase() {}
		private:
			void Execute(IQueueableMethod* method)
			{
				method->Execute();
			}
		};


	} // end of Lockfree namespace
	} // end of Thread namespace
}