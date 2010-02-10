#pragma once

namespace Earlgrey
{
	typedef unsigned int ThreadIdType;

	const ThreadIdType INVALID_THREAD_ID = UINT_MAX;

	const ThreadIdType MAX_IO_THREADS = 32;

	const ThreadIdType WIN_MAIN_THREAD_ID = 0;
	const ThreadIdType IO_THREAD_ID_BEGIN = 1;

	struct Anonymous
	{
		static ThreadIdType TID_IO_THREAD_ID_END;
	};
	
	#define IO_THREAD_ID_END Anonymous::TID_IO_THREAD_ID_END

	const ThreadIdType MAX_THREADS = MAX_IO_THREADS + 1;

	inline BOOL IsValidIOThreadId(ThreadIdType threadId)
	{
		return threadId != INVALID_THREAD_ID
			&& threadId >= IO_THREAD_ID_BEGIN 
			&& threadId <= IO_THREAD_ID_END
			;
	}

	inline BOOL IsValidThreadId(ThreadIdType threadId)
	{
		return threadId == WIN_MAIN_THREAD_ID
			|| IsValidIOThreadId(threadId)
			;
	}
}