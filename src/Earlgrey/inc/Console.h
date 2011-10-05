#pragma once
#include "Uncopyable.h"
#include "txstring.h"

#include <Loki/Singleton.h>
#include <Loki/Threads.h> // for Loki::SingleThreaded
#include "NoLock.h"

#pragma warning(push)
#pragma warning(disable: 4996)
#include <locale>
#pragma warning(pop)


namespace Earlgrey
{
	//! \ref http://serious-code.net/moin.cgi/WindowsConsoleApp
	//! \ref http://www.anycoding.com/bbs/board.php?bo_table=acProgram_Cpp&wr_id=3773
	//! \ref http://dslweb.nwnexus.com/~ast/dload/guicon.htm
	class Console : private Uncopyable
	{
		friend struct Loki::CreateStatic<Console>;

		explicit Console();
		~Console();

	public:
		BOOL Open(BOOL attachExistingConsoleIfPossible = TRUE);
		void Close();

		BOOL RedirectStdIO();

		void Write(const TCHAR * const msg);
		void Write(const TCHAR * const msg, size_t msgLen);
		void WriteLine(const TCHAR * const msg);
		void WriteLine(const TCHAR * const msg, size_t msgLen);

		void WindowTitle(const TCHAR * const msg);
		_txstring WindowTitle() const;
	
	private:
		void RedirectStdIO(DWORD nStdHandle);
		HANDLE StdHandle(DWORD nStdHandle);


	private:
		std::locale m_previousStdOutLocale;
		std::locale m_previousStdInLocale;
		std::locale m_previousStdErrLocale;
		HANDLE m_stdoutHandle;
		HANDLE m_stdinHandle;
		HANDLE m_stderrHandle;

		BOOL m_closed;
	};

	typedef 
		Loki::SingletonHolder<Console, Loki::CreateStatic, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
		gConsole;
}