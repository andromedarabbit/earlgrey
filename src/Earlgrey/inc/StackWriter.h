#pragma once 

#ifndef HAS_STACK_WALKER
#define HAS_STACK_WALKER 1 // \todo 어딘가로 옮기는 편이...
#endif

#include "txstring.h"
#include "UnhandledExceptionHandler.h"

#pragma warning( push )
#pragma warning( disable : 4100 )
#include "..\..\..\vendor\StackWalker\StackWalker.h"
#pragma warning( pop )

namespace Earlgrey
{
	//! \ref http://serious-code.net/moin.cgi/MiniDump
	//! \ref http://www.codeguru.com/cpp/w-p/system/threading/article.php/c10317 under BSD license
	class StackWriter : public UnhandledExceptionHandler
	{
	private:
		typedef StackWalker::StackWalkOptions StackWalkOptions;
		StackWalkOptions m_StackWalkOptions;
		_txstring m_LogFilePath;

	public:
		
		explicit StackWriter(const TCHAR * const logFilePath, StackWalkOptions stackWalkOptions)
			: m_LogFilePath(logFilePath)
			, m_StackWalkOptions(stackWalkOptions)
		{

		}

		explicit StackWriter(const _txstring& logFilePath, StackWalkOptions stackWalkOptions)
			: m_LogFilePath(logFilePath)
			, m_StackWalkOptions(stackWalkOptions)
		{

		}

		virtual ~StackWriter()
		{
		}

		const TCHAR * ClassName() const;

		const _txstring& FilePath() const 
		{
			return m_LogFilePath;
		}

		virtual void HandleException(LPEXCEPTION_POINTERS exceptionPtrs);	

	};
}