#pragma once
#include "lockfree.h"

namespace Earlgrey {

	namespace Lockfree {

		class ITaskBase
		{
		public:
			virtual void run() = 0;
			virtual void destory() = 0;
		};

		template<typename T>
		class Task0 : public ITaskBase
		{
		public:
			void run()
			{
				runUserDefinedFunction();
				destory();
			}

		protected:
			void destory()
			{
				delete (T*)this;
			}

			virtual void runUserDefinedFunction() = 0;
		};

		template<typename T, typename P1>
		class Task1 : public ITaskBase
		{
		public:
			Task1(P1 p1) : _p1(p1) {}

			void run()
			{
				runUserDefinedFunction( _p1 );
				destory();
			}

		protected:
			void destory()
			{
				delete (T*)this;
			}

			virtual void runUserDefinedFunction(P1 p1) = 0;

		private:
			P1 _p1;
		};

#define DECLARE_TASK0(name) \
		class name : public Earlgrey::Lockfree::Task0<name>	\
		{	\
		public:	\
		name() : Earlgrey::Lockfree::Task0<name>() {}	\
		private:	\
		void runUserDefinedFunction();	\
		};

#define DEFINE_TASK0(name)	\
	void name::runUserDefinedFunction()
 
#define DECLARE_TASK1(name, __ptype1) \
		class name : public Earlgrey::Lockfree::Task1<name, __ptype1>	\
		{	\
		public:	\
			name(__ptype1 p1) : Earlgrey::Lockfree::Task1<name, __ptype1>( p1 ) {}	\
		private:	\
			void runUserDefinedFunction(__ptype1 p1);	\
		};

#define DEFINE_TASK1(name, __ptype1, var1)	\
		void name::runUserDefinedFunction(__ptype1 var1)


		class TaskQueue : private Uncopyable
		{
		public:
			explicit TaskQueue() : _count(0) 
			{
			}

			void execute(ITaskBase* task)
			{
				if (CAS( &_count, 0L, 1L ))
				{
					task->run();

					if (InterlockedDecrement( &_count ) == 0)
						return;
				}
				else
				{
					_q.enqueue( task );

					// if there is a thread processing, don't do anything.
					if (InterlockedIncrement( &_count ) > 1)
						return;
				}
				executeAllTasksInQueue();
			}

		protected:
			void executeAllTasksInQueue()
			{
				_ASSERTE( _count > 0 );
				ITaskBase* queuedTask = NULL;
				do {
					bool isEmpty = _q.dequeue( queuedTask );
					_ASSERTE( !isEmpty );
					if (!isEmpty)
					{
						queuedTask->run();
					}
				} while(InterlockedDecrement( &_count ));
			}

		protected:
			Queue<ITaskBase*> _q;
			volatile LONG _count;
		};
	} // end of Lockfree namespace
}