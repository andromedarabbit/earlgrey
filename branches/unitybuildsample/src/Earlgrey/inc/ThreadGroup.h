#pragma once
// #include "Uncopyable.h"
#include "Thread.h"

namespace Earlgrey
{
	class thread_group
		: Uncopyable
	{
	public:
		~thread_group()
		{
			for(std::list<Thread*>::iterator it=threads.begin(),end=threads.end();
				it!=end;
				++it)
			{
				delete *it;
			}
		}

		template<typename F>
		Thread* create_thread(F threadfunc)
		{
			boost::lock_guard<shared_mutex> guard(m);
			std::auto_ptr<thread> new_thread(new thread(threadfunc));
			threads.push_back(new_thread.get());
			return new_thread.release();
		}

		void add_thread(Thread* thrd)
		{
			if(thrd)
			{
				boost::lock_guard<shared_mutex> guard(m);
				threads.push_back(thrd);
			}
		}

		void remove_thread(Thread* thrd)
		{
			boost::lock_guard<shared_mutex> guard(m);
			std::list<Thread*>::iterator const it=std::find(threads.begin(),threads.end(),thrd);
			if(it!=threads.end())
			{
				threads.erase(it);
			}
		}

		void join_all()
		{
			boost::shared_lock<shared_mutex> guard(m);

			for(std::list<Thread*>::iterator it=threads.begin(),end=threads.end();
				it!=end;
				++it)
			{
				(*it)->join();
			}
		}

		void interrupt_all()
		{
			boost::shared_lock<shared_mutex> guard(m);

			for(std::list<Thread*>::iterator it=threads.begin(),end=threads.end();
				it!=end;
				++it)
			{
				(*it)->interrupt();
			}
		}

		size_t size() const
		{
			boost::shared_lock<shared_mutex> guard(m);
			return threads.size();
		}

	private:
		std::list<Thread*> threads;
		mutable shared_mutex m;
	};
}