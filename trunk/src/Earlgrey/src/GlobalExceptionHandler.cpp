#include "stdafx.h"
#include "GlobalExceptionHandler.h"

namespace Earlgrey
{
	void GlobalExceptionHandler::Initialize()
	{
		// Catches the unhandled exception and handle it!
		LPTOP_LEVEL_EXCEPTION_FILTER previousTopLovelExceptionHandler 
			= ::SetUnhandledExceptionFilter(HandleException);

		DBG_UNREFERENCED_LOCAL_VARIABLE(previousTopLovelExceptionHandler);
	}


	void GlobalExceptionHandler::Register(HandlerPtrType handler)
	{
		m_Handlers.push_back(handler);
	}

	LONG WINAPI GlobalExceptionHandler::HandleException(LPEXCEPTION_POINTERS exceptionPtr)
	{
		HandlerCollectionType::const_iterator it = m_Handlers.begin();

		for( ; it != m_Handlers.end(); it++)
		{
			(*it)->HandleException(exceptionPtr);
		}

		// Return from UnhandledExceptionFilter and execute the associated exception handler. 
		// This usually results in process termination.
		return EXCEPTION_EXECUTE_HANDLER; 
	}
}