#ifndef _EARLGREY_UNHANDLED_EXCEPTION_HANDLER_H__
#define _EARLGREY_UNHANDLED_EXCEPTION_HANDLER_H__

#include "Uncopyable.h"

namespace Earlgrey
{	
	class UnhandledExceptionHandler : private Uncopyable
	{
	public:
		typedef std::tr1::shared_ptr<UnhandledExceptionHandler> HandlerPtrType;

	public: // public methods
		virtual void HandleException(LPEXCEPTION_POINTERS exceptionPtr) = 0;
	};

}


#endif