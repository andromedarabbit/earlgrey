#include "stdafx.h"
#include "GlobalExceptionHandler.h"

namespace Earlgrey
{
	void GlobalExceptionHandler::Initialize()
	{
		// Catches the unhandled exception and handle it!
		LPTOP_LEVEL_EXCEPTION_FILTER previousTopLovelExceptionHandler 
			= ::SetUnhandledExceptionFilter(HandleException);

		m_Initialized = TRUE;
		DBG_UNREFERENCED_LOCAL_VARIABLE(previousTopLovelExceptionHandler);
	}

	BOOL GlobalExceptionHandler::Initialized()
	{
		return m_Initialized;
	}

	void GlobalExceptionHandler::Register(HandlerPtrType handler)
	{
		EARLGREY_ASSERT(handler != NULL);
		m_Handlers.push_back(handler);
	}

	void GlobalExceptionHandler::UnregisterAll()
	{
		m_Handlers.clear();
	}

	LONG WINAPI GlobalExceptionHandler::HandleException(LPEXCEPTION_POINTERS exceptionPtr)
	{
		GlobalExceptionHandlerSingleton::Instance().InvokeAllHandler( exceptionPtr );
		
		// Return from UnhandledExceptionFilter and execute the associated exception handler. 
		// This usually results in process termination.
		return EXCEPTION_EXECUTE_HANDLER; 
	}

	void GlobalExceptionHandler::InvokeAllHandler(LPEXCEPTION_POINTERS exceptionPtr)
	{
		HandlerCollectionType::const_iterator it = m_Handlers.begin();

		for( ; it != m_Handlers.end(); it++)
		{
			(*it)->HandleException(exceptionPtr);
		}
	}

	GlobalExceptionHandler::GlobalExceptionHandler() : m_Initialized(FALSE)
	{

	}

}