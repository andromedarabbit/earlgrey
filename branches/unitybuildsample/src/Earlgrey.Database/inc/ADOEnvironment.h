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
			// 스레드 별로 초기화를 하므로 mutex가 필요 없을 듯 하다.
			static __declspec(thread) BOOL s_Initialized; 
			// static mutex_type s_InternalSyncObject; 
		};
		
	}
}