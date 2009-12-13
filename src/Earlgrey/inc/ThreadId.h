#pragma once

namespace Earlgrey
{
	typedef unsigned int ThreadIdType;

	const ThreadIdType INVALID_THREAD_ID = UINT_MAX;

	const ThreadIdType MAX_IO_THREADS = 32;

	const ThreadIdType WIN_MAIN_THREAD_ID = 0;
	const ThreadIdType IO_THREAD_ID_BEGIN = 1;
	const ThreadIdType IO_THREAD_ID_END = MAX_IO_THREADS;

	const ThreadIdType MAX_THREADS = MAX_IO_THREADS + 1;

	inline BOOL IsValidIOThreadId(ThreadIdType threadId)
	{
		return threadId >= IO_THREAD_ID_BEGIN 
			&& threadId <= IO_THREAD_ID_END
			;
	}
}