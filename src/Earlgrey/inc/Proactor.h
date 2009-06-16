#pragma once 

#include <Loki/Singleton.h>
#include <Loki/Threads.h> // for Loki::SingleThreaded
#include "NoLock.h"
#include "StlCustom.h"

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

	//Windows IOCompletion Port ��
	class Proactor
	{
	public:

		enum {
			DefaultTimeout = INFINITE
		};

		explicit Proactor() {}
		~Proactor() {}

		virtual BOOL HandleEvent(TimeValueType WaitTime) = 0;
		virtual BOOL RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler) = 0;
		virtual BOOL DeregisterHandler(HANDLE Handle) = 0;
	};

	class WinProactor 
		: public Proactor
	{
	public:
		explicit WinProactor() {}
		virtual ~WinProactor() {}

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

	// Windows WaitEvent ��
	class AcceptProactor
		: public Proactor
	{
	public:
		explicit AcceptProactor() {};
		virtual ~AcceptProactor() {};

		BOOL Initialize() {}

		//Proactor Pattern Interface
		virtual BOOL HandleEvent(TimeValueType WaitTime = DefaultTimeout);
		virtual BOOL RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler);
		virtual BOOL DeregisterHandler(HANDLE Handle);

		typedef std::map<WSAEVENT, WaitEventHandler*> EventHandlerMapType;
		typedef std::vector<WSAEVENT> EventVectorType;


	private:
		EventVectorType			_Events;
		EventHandlerMapType		_EventHandlerMap;
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
		~CompletionHandler() {};

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
			: TransferredBytes_(0), Error_(0), Status_(0), Handler_(InHandler)
		{
			Internal = 0;
			InternalHigh = 0;
			Pointer = 0;
			hEvent = 0;
		};

		virtual ~AsyncResult() {};

		virtual void Completed() = 0;
		virtual void Failed() = 0;
		virtual void TimeOut() = 0;

		void TransferredBytes(DWORD bytes) { TransferredBytes_ = bytes; };
		DWORD TransferredBytes() { return TransferredBytes_; }
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