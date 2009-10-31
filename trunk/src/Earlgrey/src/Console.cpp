#include "stdafx.h"
#include "Console.h"
#include "Environment.h"
#include "numeric_cast.hpp"

#include "Log.h"

#include <fcntl.h>
#include <io.h>


namespace Earlgrey
{
	Console::Console()
		: m_stdoutHandle(INVALID_HANDLE_VALUE)
		, m_stdinHandle(INVALID_HANDLE_VALUE)
		, m_stderrHandle(INVALID_HANDLE_VALUE)
		, m_closed(TRUE)
	{
		/*
		using namespace std::tr1::placeholders;

		std::tr1::function<BOOL (DWORD)> consoleCtrlBound = std::tr1::bind(
				std::tr1::mem_fn(&Console::ConsoleCtrlHandler), std::tr1::ref(*this), _1
			);

		void * ptr = consoleCtrlBound.target<BOOL (DWORD)>( );;
		consoleCtrlBound(1);
		DBG_UNREFERENCED_LOCAL_VARIABLE(ptr);
		PHANDLER_ROUTINE consoleCtrlFunc = consoleCtrlBound.target<BOOL  (DWORD)>( );
		EARLGREY_ASSERT(consoleCtrlFunc != NULL);


		BOOL (WINAPI Console::*funcPtr)(DWORD) = &Console::ConsoleCtrlHandler;
		((*this).*funcPtr)(2);
		consoleCtrlFunc = (PHANDLER_ROUTINE) ((*this).*funcPtr);
		::SetConsoleCtrlHandler(consoleCtrlFunc, TRUE); // TODO: ���߿� �ڵ鷯 �����ؾ� �Ѵ�.
*/
	}
/*
	BOOL WINAPI Console::ConsoleCtrlHandler(DWORD ctrlType)
	{
		switch( ctrlType ) {
			case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate			
				// gpTheService->OnCtrlBreak();
				return TRUE;

			case CTRL_C_EVENT:
			case CTRL_CLOSE_EVENT:
			case CTRL_LOGOFF_EVENT:
			case CTRL_SHUTDOWN_EVENT:
				// _tprintf(TEXT("Stopping %s.\n"), gpTheService->m_lpDisplayName);
				// gpTheService->Stop();
				exit(EXIT_SUCCESS);
			}
		return FALSE;
	}*/
	/*
	BOOL
	(WINAPI *PHANDLER_ROUTINE)(
	DWORD CtrlType
	);*/


	Console::~Console()
	{
		Close();
	}

	BOOL Console::Open()
	{
		if (::AllocConsole() == FALSE) 
		{
			// \todo ���� ���� ó���� �ʿ��ϴ�.
			DWORD errCode = GetLastError();

			_txstring errMsg = Log::ErrorMessage(errCode);
			DBG_UNREFERENCED_LOCAL_VARIABLE(errMsg);

			return FALSE;
		}

		m_stdoutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
		EARLGREY_ASSERT(m_stdoutHandle != INVALID_HANDLE_VALUE);

		m_stdinHandle = ::GetStdHandle(STD_INPUT_HANDLE);
		EARLGREY_ASSERT(m_stdinHandle != INVALID_HANDLE_VALUE);

		m_stderrHandle = ::GetStdHandle(STD_ERROR_HANDLE);		
		EARLGREY_ASSERT(m_stderrHandle != INVALID_HANDLE_VALUE);

		m_closed = FALSE;
		return TRUE;
	}

	void Console::Close()
	{
		if(m_closed)
			return;

		m_closed = TRUE;

		// \todo �Ʒ� �Լ����� ������ ���� ������ ����� ���� �ʳ�?
		::FreeConsole();
		
		::CloseHandle(m_stdoutHandle);
		m_stdoutHandle = INVALID_HANDLE_VALUE;
		
		::CloseHandle(m_stdinHandle);
		m_stdinHandle = INVALID_HANDLE_VALUE;

		::CloseHandle(m_stderrHandle);
		m_stderrHandle = INVALID_HANDLE_VALUE;
	}

	void Console::Write(const TCHAR * const msg)
	{		
		Write(msg, _tcslen(msg));
	}

	void Console::Write(const TCHAR * const msg, size_t msgLen)
	{
		EARLGREY_ASSERT(m_closed == FALSE);
		EARLGREY_ASSERT(msg != NULL);
		EARLGREY_ASSERT(msgLen > 0);

		const DWORD numberOfCharsToWrite = Math::numeric_cast<DWORD>(msgLen);
		DWORD numberOfCharsWritten = 0;
		if(::WriteConsole(m_stdoutHandle, msg, numberOfCharsToWrite,  &numberOfCharsWritten, NULL) == FALSE)
		{
			// TODO
		}
	}

	void Console::WriteLine(const TCHAR * const msg)
	{
		WriteLine(msg, _tcslen(msg));
	}

	void Console::WriteLine(const TCHAR * const msg, size_t msgLen)
	{
		static const TCHAR * const newline = Environment::NewLine();
		static const size_t newlineLength = _tcslen(newline);

		EARLGREY_ASSERT(m_closed == FALSE);
		EARLGREY_ASSERT(msg != NULL);
		EARLGREY_ASSERT(msgLen > 0);
		
		Write(msg, msgLen);
		Write(newline, newlineLength);
	}

	void Console::WindowTitle(const TCHAR * const msg)
	{
		EARLGREY_ASSERT(m_closed == FALSE);
		EARLGREY_ASSERT(msg != NULL);

		if(::SetConsoleTitle(msg) == FALSE)
		{
			// TODO: GetLastErro
			// If the buffer is not large enough to store the title, the return value is zero and GetLastError returns ERROR_SUCCESS.
			// If the function fails, the return value is zero and GetLastError returns the error code.
			throw std::exception("");
		}
	}

	_txstring Console::WindowTitle() const
	{
		TCHAR title[MAX_PATH];
		if(GetConsoleTitle(title, _countof(title)) == FALSE)
		{
			// TODO: GetLastError
			throw std::exception("");
		}

		return _txstring(title);
	}

	namespace 
	{
		const char * GetMode(DWORD nStdHandle)
		{
			if(nStdHandle == STD_OUTPUT_HANDLE)
				return "w";

			if(nStdHandle == STD_INPUT_HANDLE)
				return "r";

			if(nStdHandle == STD_ERROR_HANDLE)
				return "w";

			// TODO
			throw std::invalid_argument("");
		}

		FILE * GetStdFile(DWORD nStdHandle)
		{
			if(nStdHandle == STD_OUTPUT_HANDLE)
				return stdout;

			if(nStdHandle == STD_INPUT_HANDLE)
				return stdin;

			if(nStdHandle == STD_ERROR_HANDLE)
				return stderr;

			// TODO
			throw std::invalid_argument("");
		}
	}
	
	HANDLE Console::StdHandle(DWORD nStdHandle)
	{
		if(nStdHandle == STD_OUTPUT_HANDLE)
			return m_stdoutHandle;

		if(nStdHandle == STD_INPUT_HANDLE)
			return m_stdinHandle;

		if(nStdHandle == STD_ERROR_HANDLE)
			return m_stderrHandle;

		// TODO
		throw std::invalid_argument("");
	}

	//! \todo Close�� �ϸ� Redirect ������ �����ؾ� �Ѵ�.
	void Console::RedirectStdIO(DWORD nStdHandle)
	{
		EARLGREY_ASSERT(
			nStdHandle == STD_OUTPUT_HANDLE
			|| nStdHandle == STD_INPUT_HANDLE
			|| nStdHandle == STD_ERROR_HANDLE
			);

		long  stdHandle = 0;
		int   conHandle = 0;
		FILE* fp        = NULL;

		const char * mode = GetMode(nStdHandle);

		stdHandle = PtrToLong(StdHandle(nStdHandle));
		conHandle = _open_osfhandle(stdHandle, _O_TEXT); // �� �ڵ� �� �ݾƵ� �ǳ�?
		EARLGREY_ASSERT(conHandle != -1);

		fp = _fdopen(conHandle, mode);
		EARLGREY_ASSERT(fp != NULL);
		
		FILE * stdFile = GetStdFile(nStdHandle);
		*stdFile = *fp;
		setvbuf(stdFile, NULL, _IONBF, 0);

	}

	BOOL Console::RedirectStdIO()
	{
		EARLGREY_ASSERT(m_closed == FALSE);

		RedirectStdIO(STD_OUTPUT_HANDLE);
		RedirectStdIO(STD_INPUT_HANDLE);
		RedirectStdIO(STD_ERROR_HANDLE);

		// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
		std::ios::sync_with_stdio();

		return TRUE;
	}
}