#pragma once
#include "Uncopyable.h"
// #include <Loki/Singleton.h>
// #include <Loki/Threads.h> // for Loki::SingleThreaded
// #include "NoLock.h"
#include "txstring.h"

namespace Earlgrey
{
	//! \ref http://serious-code.net/moin.cgi/WindowsConsoleApp
	class Console : private Uncopyable
	{
		// friend struct Loki::CreateStatic<Console>;

	public:
		explicit Console();
		~Console();

		BOOL Open();
		void Close();

		BOOL RedirectStdIO();

		void Write(const TCHAR * const msg);
		void Write(const TCHAR * const msg, size_t msgLen);
		void WriteLine(const TCHAR * const msg);
		void WriteLine(const TCHAR * const msg, size_t msgLen);

		void WindowTitle(const TCHAR * const msg);
		_txstring WindowTitle() const;
	
	private:
		// HANDLE GetStdIOHandle(DWORD nStdHandle);
		void RedirectStdIO(DWORD nStdHandle);
		HANDLE StdHandle(DWORD nStdHandle);


		// static BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType);

	private:
		HANDLE m_stdoutHandle;
		HANDLE m_stdinHandle;
		HANDLE m_stderrHandle;

		BOOL m_closed;
	};

	//typedef 
	//	Loki::SingletonHolder<Console, Loki::CreateStatic, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
	//	gConsole;
}