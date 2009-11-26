#pragma once 

#include <Loki/Singleton.h>
#include <Loki/Threads.h> // for Loki::SingleThreaded
#include <vector>
#include <map>

#include "NoLock.h"

namespace Earlgrey
{
	class NetworkBuffer;
	class AsyncResult;

	typedef DWORD TimeValueType;

	typedef enum IOCP_EVENT_TYPE
	{
		READ_EVENT,
		WRITE_EVENT
	};

	class CompletionHandler;

	//Windows IOCompletion Port 용
	class Proactor   
	{
	public:

		enum {
			DefaultTimeout = INFINITE
		};

		Proactor() {}
		virtual ~Proactor() {}

		virtual BOOL HandleEvent(TimeValueType WaitTime) = 0;

		//! \todo 이렇게 운영체제 핸들을 드러내놓고 주고 받아도 되나?  
		virtual BOOL RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler) = 0;
		virtual BOOL DeregisterHandler(HANDLE Handle) = 0;
	};

	class WinProactor 
		: public Proactor
	{
	public:
		WinProactor() {}
		virtual ~WinProactor() {}

		BOOL Initialize();
		
		//Proactor Pattern Interface
		//! \todo TimeValueType의 단위를 한눈에 알아보기 힘들다. TimeSpan 적용하기
		virtual BOOL HandleEvent(TimeValueType WaitTime = DefaultTimeout); 
		virtual BOOL RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler);
		virtual BOOL DeregisterHandler(HANDLE Handle);

		BOOL PostEvent(AsyncResult* Result);

	private:
		HANDLE _IOCompletionPort;
	};

	typedef
		Loki::SingletonHolder<WinProactor, Loki::CreateUsingNew, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
			ProactorSingleton
			;

	// Windows WaitEvent 용
	/*class AcceptProactor
		: public Proactor
	{
	public:
		AcceptProactor() 
		{
			InitializeCriticalSection(&Lock_);
		};
		virtual ~AcceptProactor() {};

		BOOL Initialize() {}

		//Proactor Pattern Interface
		virtual BOOL HandleEvent(TimeValueType WaitTime = DefaultTimeout);
		virtual BOOL RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler);
		virtual BOOL DeregisterHandler(HANDLE Handle);


	private:
		typedef std::map<WSAEVENT, WaitEventHandler*> EventHandlerMapType; // TODO xmap?
		typedef std::vector<WSAEVENT> EventVectorType; // TODO xvector?


		CRITICAL_SECTION		Lock_; // TODO shared_ptr 및 lock wrapper 필요
		EventVectorType			_Events;
		EventHandlerMapType		_EventHandlerMap;
	};

	typedef
		Loki::SingletonHolder<AcceptProactor, Loki::CreateUsingNew, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
		AcceptProactorSingleton
		;
*/

	class AsyncResult;

	class CompletionHandler
	{
	public:
		explicit CompletionHandler() {};
		virtual ~CompletionHandler() {};

		virtual void HandleEvent(AsyncResult* Result, DWORD TransferredBytes) = 0;
		virtual void HandleEventError(AsyncResult* Result, DWORD Error) = 0;
	};

	/*class WaitEventHandler
		: public CompletionHandler
	{
	public:
		explicit WaitEventHandler() {};
		virtual ~WaitEventHandler() {};

		virtual void HandleEvent(AsyncResult* Result, DWORD TransferredBytes) = 0;
		virtual void HandleEventError(AsyncResult* Result, DWORD Error) = 0;

		virtual void HandleEvent() = 0;
	};*/

	class AsyncStream;

	class AsyncResult 
		: public OVERLAPPED
	{
	public:
		explicit AsyncResult(CompletionHandler* InHandler, AsyncStream* InStream = NULL) 
			: TransferredBytes_(0)
			, Error_(0)
			, Status_(0)
			, Handler_(InHandler)
			, Stream_(InStream)
		{
			Internal = 0;
			InternalHigh = 0;
			Pointer = NULL;
			hEvent = NULL;
		};

		virtual ~AsyncResult() {}

		virtual void Completed()
		{
			EARLGREY_ASSERT(Handler_ != NULL);
			Handler_->HandleEvent(this, TransferredBytes_);
		}

		virtual void Failed()
		{
			EARLGREY_ASSERT(Handler_ != NULL);
			Handler_->HandleEventError(this, Error_);
		}

		virtual void TimeOut()
		{

		}

		inline void TransferredBytes(DWORD bytes) { TransferredBytes_ = bytes; };
		inline DWORD TransferredBytes() { return TransferredBytes_; }
		inline void Status(DWORD status) { Status_ = status; };
		inline void Error(DWORD error) { Error_ = error; };
		inline DWORD Error() { return Error_; }

		inline CompletionHandler* Handler() const
		{
			return Handler_;
		}
		inline AsyncStream* Stream() const
		{
			return Stream_;
		}

	private:
		//OVERLAPPED	Overlapped_;
		DWORD TransferredBytes_;
		DWORD Error_;
		DWORD Status_;
		CompletionHandler* Handler_;
		AsyncStream* Stream_;
	};
}