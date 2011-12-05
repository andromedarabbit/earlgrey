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

		static _txstring BaseDirectory(); //! �������α׷��� ���̳ʸ� ������ ��ġ�� ��

		//! \todo Directory Ŭ������ �����ϰ� �Ǹ� �� �ʿ� �ű�� �̰� �ڵ�� �� �Լ��� ȣ���Ѵ�.
		static _txstring CurrentDirectory(); 
		static BOOL CurrentDirectory(const _txstring& pathName) throw(...);
		
		static _txstring SystemDirectory(); 


		//! ���� ��ǻ���� NetBIOS �̸��� �����´�.
		static _txstring MachineName(); 

		//! ���� ����ڿ� ���õ� ��Ʈ��ũ ������ �̸��� �����´�.
		static _txstring UserDomainName(); 

		// static _txstring OSVersion();

		static DWORD TickCount();

		// ���⼭���� .NET Framework�� �ٸ� �޼����
		static BOOL UserInteractive() throw(...); 

		static __int64 GetPerformanceCounter();

		static __int64 GetPerformanceFrequency();

		static ULONGLONG TickCount64();

	};
}
