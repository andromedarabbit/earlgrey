#pragma once 

#include <Loki/Singleton.h>
#include <Loki/Threads.h> // for Loki::SingleThreaded
#include "NoLock.h"
#include "StlCustom.h"
#include "EarlgreyAssert.h"

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
	class WaitEventHandler;

	//Windows IOCompletion Port ¿ë
	class Proactor
	{
	public:

		enum {
			DefaultTimeout = INFINITE
		};

		virtual BOOL HandleEvent(TimeValueType WaitTime) = 0;
		virtual BOOL RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler) = 0;
		virtual BOOL DeregisterHandler(HANDLE Handle) = 0;
	};

	class WinProactor 
		: public Proactor
	{
	public:
		BOOL Initialize();
		
		//Proactor Pattern Interface
		virtual BOOL HandleEvent(TimeValueType WaitTime = DefaultTimeout);
		virtual BOOL RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler);
		virtual BOOL DeregisterHandler(HANDLE Handle);

		BOOL PostEvent(CompletionHandler* CompleteHandler, AsyncResult* ResultStream);

	private:
		HANDLE _IOCompletionPort;
	};

	typedef
		Loki::SingletonHolder<WinProactor, Loki::CreateUsingNew, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
			ProactorSingleton
			;

	// Windows WaitEvent ¿ë
	class AcceptProactor
		: public Proactor
	{
	public:
		BOOL Initialize() {}

		//Proactor Pattern Interface
		virtual BOOL HandleEvent(TimeValueType WaitTime = DefaultTimeout);
		virtual BOOL RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler);
		virtual BOOL DeregisterHandler(HANDLE Handle);

	private:
		std::vector<WSAEVENT> Events;//!TODO : 
		std::vector<WaitEventHandler*> EventHandlers;
	};

	typedef
		Loki::SingletonHolder<AcceptProactor, Loki::CreateUsingNew, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
		AcceptProactorSingleton
		;


	class AsyncResult;

	class CompletionHandler
	{
	public:
		explicit CompletionHandler() {};
		virtual ~CompletionHandler() {};

		virtual void HandleEvent(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* Result) = 0; 
		virtual HANDLE GetHandle() = 0;
	};

	class WaitEventHandler
		: public CompletionHandler
	{
	public:
		explicit WaitEventHandler() {};
		virtual ~WaitEventHandler() {};

		virtual void HandleEvent(HANDLE /*Handle*/, IOCP_EVENT_TYPE /*Type*/, AsyncResult* /*Result*/) {}
		virtual void HandleEvent() = 0;
	};


	class AsyncResult 
		: public OVERLAPPED
	{
	public:
		explicit AsyncResult(CompletionHandler* InHandler) 
		{
			Handler_ = InHandler;
		};

		virtual ~AsyncResult() {};

		virtual void Completed() = 0;
		virtual void Failed() = 0;
		virtual void TimeOut() = 0;

		void TransferredBytes(DWORD bytes) { TransferredBytes_ = bytes; };
		void Status(DWORD status) { Status_ = status; };
		void Error(DWORD error) { Error_ = error; };

	protected:
		DWORD TransferredBytes_;
		DWORD Error_;
		DWORD Status_;
		CompletionHandler* Handler_;
	};

	class AsyncWriteResult
		: public AsyncResult
	{
	public:
		explicit AsyncWriteResult(CompletionHandler* InHandler)
			: AsyncResult(InHandler)
		{};

		virtual ~AsyncWriteResult() {};

		virtual void Completed()
		{
			Handler_->HandleEvent(Handler_->GetHandle(), WRITE_EVENT, this);
		}

		virtual void Failed()
		{
			//log
			return;
		}

		virtual void TimeOut()
		{

		}

	};

	class AsyncReadResult
		: public AsyncResult
	{
	public: 
		explicit AsyncReadResult(CompletionHandler* InHandler)
			: AsyncResult(InHandler)
		{};

		virtual ~AsyncReadResult() {};

		virtual void Completed()
		{
			Handler_->HandleEvent(Handler_->GetHandle(), READ_EVENT, this);
		}

		virtual void Failed()
		{
			return;
		}

		virtual void TimeOut()
		{

		}

	};
}