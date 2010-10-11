#pragma once
#include "Uncopyable.h"
#include "txstring.h"

namespace Earlgrey
{
	namespace ADO
	{
		class Environment : private Uncopyable
		{
		private:
			// typedef Loki::Mutex mutex_type;
			// typedef ScopedLock<mutex_type> scoped_lock_type;

			explicit Environment();

		public:
			static _txstring MDACVersion();
			static void InitializeADO();
			static void UninitializeADO(); 

		private:
			// ������ ���� �ʱ�ȭ�� �ϹǷ� mutex�� �ʿ� ���� �� �ϴ�.
			static __declspec(thread) BOOL s_Initialized; 
			// static mutex_type s_InternalSyncObject; 
		};
		
	}
}