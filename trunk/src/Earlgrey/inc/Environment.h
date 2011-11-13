#pragma once
#include "Uncopyable.h"
#include "txstring.h"
#include "Lock.h"

#include <Loki/Threads.h>

namespace Earlgrey 
{
	class Environment : private Uncopyable
	{
	private:
		explicit Environment();

		//typedef Loki::Mutex mutex_type;
		//typedef ScopedLock<mutex_type> scoped_lock_type;

		//static BOOL s_Initialized;
		//static mutex_type s_InternalSyncObject;

	public:
		static const TCHAR* const NewLine();

		static _txstring BaseDirectory(); //! 응용프로그램의 바이너리 파일이 위치한 곳

		//! \todo Directory 클래스를 개발하게 되면 그 쪽에 옮기고 이곳 코드는 그 함수를 호출한다.
		static _txstring CurrentDirectory(); 
		static BOOL CurrentDirectory(const _txstring& pathName) throw(...);
		
		static _txstring SystemDirectory(); 


		//! 로컬 컴퓨터의 NetBIOS 이름을 가져온다.
		static _txstring MachineName(); 

		//! 현재 사용자와 관련된 네트워크 도메인 이름을 가져온다.
		static _txstring UserDomainName(); 

		// static _txstring OSVersion();

		static DWORD TickCount();

		// 여기서부턴 .NET Framework와 다른 메서드들
		static BOOL UserInteractive() throw(...); 

		static __int64 GetPerformanceCounter();

		static __int64 GetPerformanceFrequency();

		static ULONGLONG TickCount64();

	};
}
