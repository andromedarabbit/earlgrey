#pragma once
#include "Uncopyable.h"
#include "UnhandledExceptionHandler.h"
#include "ThreadId.h"
// #include "TimeSpan.h"

#include <list>

namespace Earlgrey
{
	class TimeSpan;

	class AppSettings : private Uncopyable
	{
	public:
		typedef std::tr1::shared_ptr<UnhandledExceptionHandler> UnhandledExceptionHandlerPtr;
		typedef std::list<UnhandledExceptionHandlerPtr> UnhandledExceptionCollection;
		typedef std::tr1::shared_ptr<UnhandledExceptionCollection> UnhandledExceptionCollectionPtr;

		explicit AppSettings() {}
		virtual ~AppSettings() {}

		virtual BOOL AllowOnlyOneInstance() const = NULL;
		virtual DWORD NumberOfIOThreads() const = NULL;
		virtual DWORD NumberOfConcurrentIOThreads() const = NULL;
		virtual const WCHAR * const ShortName() const = NULL;
		virtual const WCHAR * const LongName() const = NULL;
		virtual const WCHAR * const Description() const = NULL;
		virtual const WCHAR * const Version() const = NULL;
		
		virtual UnhandledExceptionCollectionPtr UnhandledExceptions() const = NULL;

		// virtual ThreadIdType LoggingThreadId() const = NULL;
		virtual BOOL IsThisLoggingThread() const = NULL;
		virtual TimeSpan LoggingFlushInterval() const = NULL;
	};

}