#pragma once
#include "EarlgreyAssert.h"


namespace Earlgrey
{
	namespace Algorithm
	{
		namespace Lockfree
		{
			class IQueueableMethod 
			{
			public:
				//! This is a hook method will be called when a task is dequeued.
				// virtual void Execute(class TaskQueueClassBase* taskQueue) = 0;
				virtual void Execute() = 0;
			};


			template<typename TaskQueueT, class _Fty>
			class SyncdMethod
			{
			public:
				SyncdMethod(TaskQueueT* task, std::tr1::function<_Fty> func)
					: m_Task(task)
					, m_Function( func )
				{
					EARLGREY_ASSERT(m_Task != NULL);

				}

				SyncdMethod& operator = (const SyncdMethod& obj)
				{
					return SyncdMethod(obj.m_Task, obj.m_Function);

				}

				class TaskRequest : public IQueueableMethod   
				{
				public:   
					explicit TaskRequest(std::tr1::function<_Fty> function)
						: m_Function(function)
					{
					}

					void Execute()
					{
						m_Function();
						delete this; // TODO: auto_ptr 로 해결하기
					}

				private:   
					std::tr1::function<_Fty> m_Function;
				};   

				void Execute()
				{
					if (Earlgrey::Algorithm::CAS( &m_Task->_count, 0L, 1L ))
					{
						m_Function();
						if (InterlockedDecrement( &m_Task->_count ) == 0)
							return;   
					}
					else
					{   
						m_Task->_q.Enqueue( new TaskRequest(m_Function) );   
						if (InterlockedIncrement( &m_Task->_count ) > 1)
							return;   
					}   
					m_Task->ExecuteAllTasksInQueue();

				}

			private:
				TaskQueueT* m_Task;
				std::tr1::function<_Fty> m_Function;

			};
		}
	}
}
