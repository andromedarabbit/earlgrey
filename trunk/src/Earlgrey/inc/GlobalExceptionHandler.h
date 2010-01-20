#pragma once
#include <vector>

#include "UnhandledExceptionHandler.h"
#include "Uncopyable.h"

namespace Earlgrey
{
	class GlobalExceptionHandler : private Uncopyable
	{
	private:
		typedef UnhandledExceptionHandler::HandlerPtrType HandlerPtrType;
		typedef std::vector<HandlerPtrType> HandlerCollectionType;

		explicit GlobalExceptionHandler();

		static HandlerCollectionType m_Handlers;


	public: // public methods
		static void Initialize();
		static void Register(HandlerPtrType handler);

	private: // private methods
		static LONG WINAPI HandleException(LPEXCEPTION_POINTERS exceptionPtr);
	};
}

