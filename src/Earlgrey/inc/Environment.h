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

		typedef Loki::Mutex mutex_type;
		typedef ScopedLock<mutex_type> scoped_lock_type;

		enum OSName
		{
			OSNAME_INVALID = 0,
			OSNAME_NT4 = 0x81,
			OSNAME_UNKNOWN = 1,
			OSNAME_WIN2K = 130,
			OSNAME_WIN95 = 0x41,
			OSNAME_WIN98 = 0x42,
			OSNAME_WIN9X = 0x40,
			OSNAME_WINME = 0x43,
			OSNAME_WINNT = 0x80
		};

		// static BOOL s_Initialized;
		static mutex_type s_InternalSyncObject;
		static OSName m_osname;

	public:
		static const TCHAR* const NewLine();

		static DWORD ProcessorCount();

		static _txstring BaseDirectory(); //! �������α׷��� ���̳ʸ� ������ ��ġ�� ��

		//! \todo Directory Ŭ������ �����ϰ� �Ǹ� �� �ʿ� �ű�� �̰� �ڵ�� �� �Լ��� ȣ���Ѵ�.
		static _txstring CurrentDirectory(); 
		static BOOL CurrentDirectory(const _txstring& pathName) throw(...);
		
		//! ���� ��ǻ���� NetBIOS �̸��� �����´�.
		static _txstring MachineName(); 

		//! ���� ����ڿ� ���õ� ��Ʈ��ũ ������ �̸��� �����´�.
		static _txstring UserDomainName(); 

		static _txstring OSVersion();

		// ���⼭���� .NET Framework�� �ٸ� �޼����
		static BOOL Is_WinXP_SP2_or_Later();
		static BOOL Is_Win_Server();

		static BOOL Is64BitWindows();

		static BOOL IsLittleEndian();

		static WORD ProcessorCacheLineSize();
		static DWORD ActiveProcessorCoresCount();

		static BOOL UserInteractive();

		static DWORD TickCount();
		static ULONGLONG TickCount64();

	private:
		static OSName OSInfo();

	};
}
