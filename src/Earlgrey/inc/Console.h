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
		BOOL Close();

		void Write(const TCHAR * const msg, DWORD msgLen);
		void WriteLine(const TCHAR * const msg, DWORD msgLen);
	public: // class methods
		// static BOOL ConsoleExists();


	private:
		HANDLE m_consoleHandle;
	};

	//typedef 
	//	Loki::SingletonHolder<Console, Loki::CreateStatic, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
	//	gConsole;
}