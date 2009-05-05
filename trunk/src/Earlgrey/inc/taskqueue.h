#pragma once
#include "lockfree.h"

namespace Earlgrey {
	namespace Thread {
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
			Task1(P1 p1) : _p1(p1) {}

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

#define DECLARE_TASK0(TaskName) \
		class TaskName : public Earlgrey::Thread::Lockfree::Task0<TaskName>	\
		{	\
		public:	\
		TaskName() : Earlgrey::Thread::Lockfree::Task0<TaskName>() {}	\
		private:	\
		void UserDefinedFunction();	\
		};

#define DEFINE_TASK(TaskName) \
	void TaskName::UserDefinedFunction
 
#define DECLARE_TASK1(TaskName, __ptype1) \
		class TaskName : public Earlgrey::Thread::Lockfree::Task1<TaskName, __ptype1>	\
		{	\
		public:	\
			TaskName(__ptype1 p1) : Earlgrey::Thread::Lockfree::Task1<TaskName, __ptype1>( p1 ) {}	\
		private:	\
			void UserDefinedFunction(__ptype1 p1);	\
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
			virtual void Execute(class TaskQueueClassBase* taskQueue) = 0;
		};

		class TaskQueueClassBase : public TaskQueueBase<IQueueableMethod>
		{
		public:
			TaskQueueClassBase() {}
			virtual ~TaskQueueClassBase() {}
		private:
			void Execute(IQueueableMethod* method)
			{
				method->Execute( this );
			}
		};

#define DECLARE_QUEUEABLE_CLASS(ClassName)	\
	typedef ClassName* QueueableClassPointerType

#define DECLARE_METHOD(MethodName)	\
		class MethodName##_Queueable : public IQueueableMethod	\
		{	\
		public:	\
			void Execute(TaskQueueClassBase* taskQueue)	\
			{	\
				QueueableClassPointerType queueableClass = (QueueableClassPointerType) taskQueue;	\
				queueableClass->Raw##MethodName();	\
				delete this;	\
			}	\
		};	\
		void MethodName()	\
		{	\
			Post( new MethodName##_Queueable() );	\
		}	\
		public:	\
		void Raw##MethodName();



	} // end of Lockfree namespace
	} // end of Thread namespace
}