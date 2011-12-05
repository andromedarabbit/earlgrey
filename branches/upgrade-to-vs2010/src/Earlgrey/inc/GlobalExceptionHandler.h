#pragma once
#include <vector>

#include "UnhandledExceptionHandler.h"
#include "Uncopyable.h"

namespace Earlgrey
{
	class GlobalExceptionHandler : private Uncopyable
	{
		typedef UnhandledExceptionHandler::HandlerPtrType HandlerPtrType;
		typedef std::vector<HandlerPtrType> HandlerCollectionType;
	public:
		explicit GlobalExceptionHandler();

		BOOL Initialized();
		void Initialize();
		void Register(HandlerPtrType handler);
		void UnregisterAll();
		void InvokeAllHandler(LPEXCEPTION_POINTERS exceptionPtr);

	private: // private methods
		static LONG WINAPI HandleException(LPEXCEPTION_POINTERS exceptionPtr);
		BOOL m_Initialized;
		HandlerCollectionType m_Handlers;
	};

	typedef Loki::SingletonHolder<
		GlobalExceptionHandler,
		Loki::CreateUsingNew,
		Loki::LongevityLifetime::DieDirectlyBeforeLast
	> GlobalExceptionHandlerSingleton;
}

