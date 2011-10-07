#include "stdafx.h"
#include "Console.h"
#include "Environment.h"
#include "numeric_cast.hpp"
#include "Log.h"

#include <fcntl.h>
#include <io.h>

#include "tiostream.h"

namespace Earlgrey
{
	namespace 
	{
		const TCHAR * GetMode(DWORD nStdHandle)
		{
			if(nStdHandle == STD_OUTPUT_HANDLE)
				return _T("w");

			if(nStdHandle == STD_INPUT_HANDLE)
				return _T("r");

			if(nStdHandle == STD_ERROR_HANDLE)
				return _T("w");

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

		int GetBufferingPolicy(size_t bufferSize, DWORD m_stdHandleType)
		{	
			EARLGREY_ASSERT(bufferSize >= 0);

			if(bufferSize == 0)
				return _IONBF;

			if(m_stdHandleType == STD_INPUT_HANDLE)
				return _IOLBF; // Windows dosen't support _IOLBF, it behaves just as like _IONBF
			
			return _IOFBF;;
		}


		class ConsoleRedirect : private Uncopyable
		{
		public:
			explicit ConsoleRedirect(DWORD stdHandleType, HANDLE stdHandle, size_t bufferSize = 0);

			~ConsoleRedirect();

			void Open();

			void Close();

			inline DWORD StdHandleType() const
			{
				return m_stdHandleType;
			}

			inline HANDLE StdHandle() const
			{
				return m_stdHandle;
			}

		private:
			BOOL m_closed;

			DWORD m_stdHandleType;
			HANDLE m_stdHandle;
			size_t m_bufferSize;

			char * m_buffer;
			int m_conHandle;
			FILE * m_previousStdHandle;
		};

		ConsoleRedirect::ConsoleRedirect(DWORD stdHandleType, HANDLE stdHandle, size_t bufferSize)
			// : m_previousLocale()
			: m_closed(TRUE)
			, m_stdHandleType(stdHandleType)
			, m_stdHandle(stdHandle)
			, m_bufferSize(bufferSize)
			, m_buffer(NULL)
			, m_conHandle(-1)
			, m_previousStdHandle(NULL)
		{
			EARLGREY_ASSERT(
				m_stdHandleType == STD_OUTPUT_HANDLE
				|| m_stdHandleType == STD_INPUT_HANDLE
				|| m_stdHandleType == STD_ERROR_HANDLE
				);		

			EARLGREY_ASSERT(m_stdHandle != INVALID_HANDLE_VALUE);

			EARLGREY_ASSERT(m_bufferSize >= 0);
		}

		ConsoleRedirect::~ConsoleRedirect()
		{
			if(m_closed == FALSE)
				Close();
		}

		void ConsoleRedirect::Open()
		{
			EARLGREY_ASSERT(m_closed == TRUE);

			const TCHAR * mode = GetMode(m_stdHandleType);

			long stdHandle = PtrToLong(m_stdHandle); // 콘솔 핸들
			m_conHandle = _open_osfhandle(stdHandle, _O_TEXT);
			EARLGREY_ASSERT(m_conHandle != -1);

			// SetStdHandle(m_stdHandleType, 
			FILE * fp = _tfdopen(m_conHandle, mode);
			EARLGREY_ASSERT(fp != NULL);

			FILE * stdFile = GetStdFile(m_stdHandleType); // 원본 핸들
			m_previousStdHandle = stdFile;
			*stdFile = *fp;

			// TODO: 옵션 바꿔서 버퍼 사용하기
			const int bufferingPolicy = GetBufferingPolicy(m_bufferSize, m_stdHandleType);
			if(m_bufferSize > 0)
				m_buffer = new char[m_bufferSize];
			EARLGREY_ASSERT(setvbuf(stdFile, m_buffer, bufferingPolicy, m_bufferSize) == 0);

			m_closed = FALSE;
		}

		void ConsoleRedirect::Close()
		{
			EARLGREY_ASSERT(m_closed == FALSE);
			
			FILE * stdFile = GetStdFile(m_stdHandleType);
			EARLGREY_ASSERT(fclose(stdFile) == 0);
			*stdFile = *m_previousStdHandle;
			
			
			// To close a file opened with _open_osfhandle, call _close. 
			// The underlying handle is also closed by a call to _close, so it is not necessary to call the Win32 function CloseHandle on the original handle.
			// _close(m_conHandle); // fp will be closed automatically
			m_conHandle = -1;

			delete m_buffer;
			m_buffer = NULL;

			m_closed = TRUE;
		}
	} //! anonymous namespace 



	Console::Console()
		: m_previousStdOutLocale()
		, m_previousStdInLocale()
		, m_previousStdErrLocale()
		, m_stdOut(NULL)
		, m_stdIn(NULL)
		, m_stdErr(NULL)
		, m_closed(TRUE)
		, m_redirected(FALSE)
	{
	}

	Console::~Console()
	{
		if(m_closed == FALSE)
			Close();
	}

	//! \todo 하드 코딩한 로케일을 어떻게 고쳐야 한다.
	BOOL Console::Open(BOOL attachExistingConsoleIfPossible)
	{
		if(m_closed == FALSE)
			return FALSE;

		m_previousStdOutLocale = _tcout.imbue( std::locale("kor") );
		m_previousStdInLocale = _tcin.imbue( std::locale("kor") );
		m_previousStdErrLocale = _tcerr.imbue( std::locale("kor") );

		BOOL consoleAttached = FALSE;

		if(attachExistingConsoleIfPossible)
			consoleAttached = ::AttachConsole(ATTACH_PARENT_PROCESS);

		if (consoleAttached == FALSE && ::AllocConsole() == FALSE) 
		{
			// \todo 뭔가 오류 처리가 필요하다.
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
		EARLGREY_ASSERT(m_closed == FALSE);

		if(m_redirected == TRUE)
			RestoreStdIO();

		// no need to close default standard i/o
		// even if you try, it will fail.

		/*EARLGREY_VERIFY(::CloseHandle(m_stdoutHandle));
		m_stdoutHandle = INVALID_HANDLE_VALUE;

		EARLGREY_VERIFY(::CloseHandle(m_stdinHandle));
		m_stdinHandle = INVALID_HANDLE_VALUE;

		EARLGREY_VERIFY(::CloseHandle(m_stderrHandle));
		m_stderrHandle = INVALID_HANDLE_VALUE;*/

		EARLGREY_VERIFY(::FreeConsole());

		_tcerr.imbue( m_previousStdErrLocale );
		_tcin.imbue( m_previousStdInLocale );
		_tcout.imbue( m_previousStdOutLocale );

		m_closed = TRUE;
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

		const DWORD numberOfCharsToWrite = EARLGREY_NUMERIC_CAST<DWORD>(msgLen);
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
			// TODO: GetLastError
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

	BOOL Console::RedirectStdIO()
	{
		EARLGREY_ASSERT(m_closed == FALSE);
		EARLGREY_ASSERT(m_redirected == FALSE);

		int bufferSize = 1024 * 1024; // \note not changable for now

		m_stdOut = new ConsoleRedirect(STD_OUTPUT_HANDLE, m_stdoutHandle, bufferSize);
		m_stdOut->Open();
		m_stdIn = new ConsoleRedirect(STD_INPUT_HANDLE, m_stdinHandle, 0);
		m_stdIn->Open();
		m_stdErr = new ConsoleRedirect(STD_ERROR_HANDLE, m_stderrHandle, 0);
		m_stdErr->Open();

		m_redirected = TRUE;

		// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
		_tios::sync_with_stdio(true);

		return TRUE;
	}

	BOOL Console::RestoreStdIO()
	{
		EARLGREY_ASSERT(m_redirected == TRUE);
			
		if(m_stdOut != NULL)
		{
			m_stdOut->Close();
			delete m_stdOut;
		}
		if(m_stdIn != NULL)
		{
			m_stdIn->Close();
			delete m_stdIn;
		}
		if(m_stdErr != NULL)
		{
			m_stdErr->Close();
			delete m_stdErr;
		}
		m_redirected = FALSE;

		return TRUE;
	}
}