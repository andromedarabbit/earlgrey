#pragma once
#include "Uncopyable.h"
#include <list>
#include "UnhandledExceptionHandler.h"

namespace Earlgrey
{
	class AppSettings : private Uncopyable
	{
	public:
		typedef std::tr1::shared_ptr<UnhandledExceptionHandler> UnhandledExceptionHandlerPtr;
		typedef std::list<UnhandledExceptionHandlerPtr> UnhandledExceptionCollection;
		typedef std::tr1::shared_ptr<UnhandledExceptionCollection> UnhandledExceptionCollectionPtr;

		explicit AppSettings() {}
		virtual ~AppSettings() {}

		virtual BOOL AllowOnlyOneInstance() const = 0;
		virtual DWORD NumberOfIOThreads() const = 0;
		virtual const TCHAR * const ShortName() const = 0;
		virtual const TCHAR * const LongName() const = 0;
		virtual const TCHAR * const Description() const = 0;
		virtual const TCHAR * const Version() const = 0;
		
		virtual UnhandledExceptionCollectionPtr UnhandledExceptions() const = 0;
	};

}