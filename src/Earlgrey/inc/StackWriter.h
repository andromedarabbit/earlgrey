#pragma once 

#ifndef HAS_STACK_WALKER
#define HAS_STACK_WALKER 1
#endif

#include "StlCustom.h"
#include "UnhandledExceptionHandler.h"
// #include "MiniDump.h"

namespace Earlgrey
{
	//! \ref http://serious-code.net/moin.cgi/MiniDump
	//! \ref http://www.codeguru.com/cpp/w-p/system/threading/article.php/c10317 under BSD license
	class StackWriter : public UnhandledExceptionHandler
	{
	private:
		// const MiniDump& m_MiniDump;
		_txstring m_LogFilePath;

	public:
		// explicit StackWriter(const MiniDump& miniDump, const TCHAR * const logFilePath)
			// : m_MiniDump(miniDump)
		explicit StackWriter(const TCHAR * const logFilePath)
			: m_LogFilePath(logFilePath)
		{

		}

		explicit StackWriter(const _txstring& logFilePath)
			: m_LogFilePath(logFilePath)
		{

		}

		virtual ~StackWriter()
		{
		}

		virtual void HandleException(LPEXCEPTION_POINTERS exceptionPtrs);	

	};
}