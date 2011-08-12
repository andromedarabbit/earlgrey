#pragma once

namespace Earlgrey {
	template<class LockType = Loki::Mutex>
	class ScopedLock
		: private Uncopyable
	{
	private:
		ScopedLock();

	public:
		typedef LockType mutex_type;

		explicit ScopedLock(mutex_type& Lock) 
			: Lock_(&Lock)
		{
			Lock_->Lock();
		}

		~ScopedLock(void)
		{
			Lock_->Unlock();
		}

	private:
		mutex_type*	Lock_;
	};

};