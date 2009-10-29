#include "stdafx.h"
#include "Console.h"
#include "Environment.h"
#include "numeric_cast.hpp"

namespace Earlgrey
{
	Console::Console()
		: m_consoleHandle(INVALID_HANDLE_VALUE)
	{

	}

	Console::~Console()
	{
		if(m_consoleHandle == INVALID_HANDLE_VALUE)
			return;

		if(Close() == FALSE)
		{
			// TODO
		}
	}

	BOOL Console::Open()
	{
		if (::AllocConsole() == FALSE) 
			return FALSE; 

		m_consoleHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
		if (m_consoleHandle == INVALID_HANDLE_VALUE) 
		{
			if(Close() == FALSE)
			{
				// TODO
			}
			return FALSE;
		}

		return TRUE;
	}

	BOOL Console::Close()
	{
		if(::FreeConsole() == FALSE
			|| ::CloseHandle(m_consoleHandle) == FALSE)
		{
			// TODO: GetLastError
			return FALSE;
		}

		m_consoleHandle = INVALID_HANDLE_VALUE;
		return TRUE;
	}

	void Console::Write(const TCHAR * const msg, DWORD msgLen)
	{
		DWORD dwBytes = 0;
		if(::WriteFile(m_consoleHandle, msg, sizeof(TCHAR) * msgLen, &dwBytes, NULL) == FALSE)
		{
			// TODO
		}
	}

	void Console::WriteLine(const TCHAR * const msg, DWORD msgLen)
	{
		using namespace Earlgrey::Math;

		static const TCHAR * const newline = Environment::NewLine();
		static const DWORD newlineLength = numeric_cast<DWORD>(_tcslen(newline));
		
		Write(msg, msgLen);
		Write(newline, newlineLength);
	}
}