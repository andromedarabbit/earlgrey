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

#define DECLARE_TASK0(TaskName) \
		class TaskName : public Earlgrey::Thread::Lockfree::Task0<TaskName>	\
		{	\
		public:	\
		explicit TaskName() : Earlgrey::Thread::Lockfree::Task0<TaskName>() {}	\
		private:	\
		void UserDefinedFunction();	\
		};

#define DEFINE_TASK(TaskName) \
	void TaskName::UserDefinedFunction
 
#define DECLARE_TASK1(TaskName, __ptype1) \
		class TaskName : public Earlgrey::Thread::Lockfree::Task1<TaskName, __ptype1>	\
		{	\
		public:	\
			explicit TaskName(__ptype1 p1) : Earlgrey::Thread::Lockfree::Task1<TaskName, __ptype1>( p1 ) {}	\
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
			virtual void Execute(class TaskQueueClassBase* taskQueue) = 0;
		};

		class TaskQueueClassBase : public TaskQueueBase<IQueueableMethod>
		{
		public:
			explicit TaskQueueClassBase() {}
			virtual ~TaskQueueClassBase() {}
		private:
			void Execute(IQueueableMethod* method)
			{
				method->Execute( this );
			}
		};

#define DECLARE_QUEUEABLE_CLASS(ClassName)	\
	typedef ClassName* QueueableClassPointerType

#define DEFINE_QUEUEABLE_METHOD(MethodName, ArgDecl, InitializationCode, MemberDecl, PassMembers)	\
	protected:	\
	class MethodName##_Queueable : public IQueueableMethod	\
	{	\
	public:	\
		MethodName##_Queueable ArgDecl InitializationCode	\
		void Execute(TaskQueueClassBase* taskQueue)	\
		{	\
			QueueableClassPointerType queueableClass = (QueueableClassPointerType) taskQueue;	\
			queueableClass->Raw##MethodName PassMembers ;	\
			delete this;	\
		}	\
	private:	\
		MemberDecl;	\
	};	

// A class derived from TaskQueueClassBase don't make use of TaskQueueBase::Post method, 
// but posting code is implemented in the wrapper method.
// when the wrapper method is called, then post into the task queue or execute the task directly.
// following code is similar to TaskQueueBase::Post method.
#define WRAPPER_METHOD_CODE(MethodName, ArgDecl, PassArgs)	\
		public:	\
		void MethodName ArgDecl	\
		{	\
			if (Earlgrey::Thread::CAS( &_count, 0L, 1L ))	{	\
				Raw##MethodName PassArgs;	\
				if (InterlockedDecrement( &_count ) == 0) return;	\
			} else {	\
				_q.Enqueue( new MethodName##_Queueable PassArgs );	\
				if (InterlockedIncrement( &_count ) > 1) return;	\
			}	\
			ExecuteAllTasksInQueue();	\
		}

// This is a real method; prefix => 'Raw'
#define DECLARE_RAW_METHOD(MethodName, ArgDecl)	\
	protected:	\
	void Raw##MethodName ArgDecl

#define DECLARE_METHOD_GENERAL(MethodName, RefArgDecl, PassArgDecl, InitializationCode, MemberDecl, PassArgs, PassMembers)	\
		DEFINE_QUEUEABLE_METHOD(MethodName, RefArgDecl, InitializationCode, MemberDecl, PassMembers)	\
		WRAPPER_METHOD_CODE(MethodName, PassArgDecl, PassArgs);	\
		DECLARE_RAW_METHOD(MethodName, PassArgDecl);

#define DECLARE_METHOD0(MethodName)	DECLARE_METHOD_GENERAL(MethodName, (), (), {},, (), ())

#define DECLARE_METHOD1(MethodName, Type1)	\
	DECLARE_METHOD_GENERAL(MethodName, (Type1& Arg1), (Type1 Arg1), { _Arg1 = Arg1; }, Type1 _Arg1, (Arg1), (_Arg1))

#define DECLARE_METHOD2(MethodName, Type1, Type2)	\
	DECLARE_METHOD_GENERAL(MethodName, (Type1& Arg1, Type2& Arg2), (Type1 Arg1, Type2 Arg2), { _Arg1 = Arg1; _Arg2 = Arg2; }, Type1 _Arg1; Type2 _Arg2, (Arg1, Arg2), (_Arg1, _Arg2))


	} // end of Lockfree namespace
	} // end of Thread namespace
}