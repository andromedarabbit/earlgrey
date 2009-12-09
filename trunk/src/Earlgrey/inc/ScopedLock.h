#pragma once

namespace Earlgrey {
	template<class LockType>
	class TScopedLock
		: private Uncopyable
	{
	private:
		TScopedLock();

	public:
		TScopedLock(LockType* Lock) : Lock_(Lock)
		{
			Lock_->Lock();
		}

		~TScopedLock(void)
		{
			Lock_->UnLock();
		}

	private:
		LockType*	Lock_;
	};

};