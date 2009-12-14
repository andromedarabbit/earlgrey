#pragma once
#include "Uncopyable.h"

namespace Earlgrey
{
	class AppSettings : private Uncopyable
	{
	public:
		explicit AppSettings() {}
		virtual ~AppSettings() {}

		virtual DWORD IOThreads() const = 0;
		virtual const TCHAR * const ShortName() const = 0;
		virtual const TCHAR * const LongName() const = 0;
		virtual const TCHAR * const Description() const = 0;
		virtual const TCHAR * const Version() const = 0;
	};

}