#pragma once
#include "Uncopyable.h"

namespace Earlgrey
{
	class OperatingSystem : private Uncopyable
	{
		friend class Application;
	
	public:
		static BOOL IsWin9x();
		static BOOL IsWinNt();
		static BOOL IsWin2K();
		static BOOL IsPostWin2K();
		static BOOL IsWinXp();
		static BOOL IsWinXpSp2();
		static BOOL IsWin2k3();
		static BOOL IsWin2k3Sp1();
		static BOOL IsVista();
		static BOOL IsWin7();
		static BOOL IsNtServer();
		static BOOL IsNtDesktop();
		static BOOL Is64BitWindows();
		// static BOOL IsWinXPSP2OrLater();

		static const TCHAR * const OSVersion() throw(...);

	private:
		explicit OperatingSystem(); // not implemented

		static void Initialize() throw(...);

	private:
		typedef Loki::Mutex mutex_type;
		typedef ScopedLock<mutex_type> scoped_lock_type;
		static mutex_type s_InternalSyncObject;

		static BOOL s_Initialized;
		
		static BOOL s_IsWin9x;
		static BOOL s_IsWinNt;
		static BOOL s_IsWin2K;
		static BOOL s_IsPostWin2K;
		static BOOL s_IsWinXp;
		static BOOL s_IsWinXpSp2;
		static BOOL s_IsWin2k3;
		static BOOL s_IsWin2k3Sp1;
		static BOOL s_IsVista;
		static BOOL s_IsWin7;
		static BOOL s_IsNtServer;
		static BOOL s_IsNtDesktop;
		static BOOL s_Is64BitWindows;

		static const size_t OS_VERSION_STRING_LENGTH = MAX_PATH;  // TODO: 충분하겠지?
		static TCHAR s_OSVersionString[OS_VERSION_STRING_LENGTH];

	};
}