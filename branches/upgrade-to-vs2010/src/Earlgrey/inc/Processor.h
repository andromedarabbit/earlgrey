#pragma once
#include "Uncopyable.h"

namespace Earlgrey
{
	class Processor : private Uncopyable
	{
		friend class Application;

	public:
		static DWORD ProcessorCount();
		static DWORD ActiveProcessorCoresCount();
		
		static WORD ProcessorCacheLineSize();
		static BOOL IsLittleEndian();

	private:
		explicit Processor();
		static void Initialize();

	private:
		typedef Loki::Mutex mutex_type;
		typedef ScopedLock<mutex_type> scoped_lock_type;
		static mutex_type s_InternalSyncObject;

		static BOOL s_Initialized;

		static DWORD s_ProcessorCount;
		static DWORD s_ActiveProcessorCoresCount;
		static WORD s_ProcessorCacheLineSize;
		static BOOL s_IsLittleEndian;
	};

}