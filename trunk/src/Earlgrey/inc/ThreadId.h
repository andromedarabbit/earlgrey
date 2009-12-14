#pragma once

namespace Earlgrey
{
	typedef unsigned int ThreadIdType;

	const ThreadIdType INVALID_THREAD_ID = UINT_MAX;

	const ThreadIdType MAX_IO_THREADS = 32;

	const ThreadIdType WIN_MAIN_THREAD_ID = 0;
	const ThreadIdType IO_THREAD_ID_BEGIN = 1;
	const ThreadIdType IO_THREAD_ID_END = MAX_IO_THREADS; // !\todo 실제로 생성한 IOCP 스레드에 맞춰 값을 반환하는 게 좋음

	const ThreadIdType MAX_THREADS = MAX_IO_THREADS + 1;

	inline BOOL IsValidIOThreadId(ThreadIdType threadId)
	{
		return threadId >= IO_THREAD_ID_BEGIN 
			&& threadId <= IO_THREAD_ID_END
			;
	}

	//! \todo 개선의 여지가 있음. 주 스레드, IOCP 스레드 외에도 커스텀 스레드가 필요한 경우가 있음. 예) 과거 캐시서버
	inline BOOL IsValidThreadId(ThreadIdType threadId)
	{
		return threadId == WIN_MAIN_THREAD_ID
			|| IsValidIOThreadId(threadId)
			;
	}
}