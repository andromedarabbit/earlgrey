#pragma once


#include "Uncopyable.h"
#include "UnhandledExceptionHandler.h"

//#include <Loki/Singleton.h>
//#include <Loki/Threads.h> // for Loki::SingleThreaded
//#include "NoLock.h"

#include <vector>

namespace Earlgrey
{
	// class UnhandledExceptionHandler;

	class GlobalExceptionHandler : private Uncopyable
	{
	private:
		// friend struct Loki::CreateStatic<UnhandledExceptionHandler>;
		typedef UnhandledExceptionHandler::HandlerPtrType HandlerPtrType;
		typedef std::vector<HandlerPtrType> HandlerCollectionType;

		explicit GlobalExceptionHandler();

		static HandlerCollectionType m_Handlers;


	public: // public methods
		void Initialize();
		void Register(HandlerPtrType handler);

	private: // private methods
		static LONG WINAPI HandleException(LPEXCEPTION_POINTERS exceptionPtr);
	};


	/*typedef 
		Loki::SingletonHolder<GlobalExceptionHandler, Loki::CreateStatic, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
		GlobalExceptionHandlerSingleton
		;*/


}

