#pragma once 

#include <Loki/Singleton.h>
#include <Loki/Threads.h> // for Loki::SingleThreaded
#include "NoLock.h"

#include "EarlgreyAssert.h"

namespace Earlgrey
{
	class NetworkBuffer;

	typedef DWORD TimeValueType;

	typedef enum IOCP_EVENT_TYPE
	{
		READ_EVENT,
		WRITE_EVENT
	};

	class CompletionHandler;

	class Proactor
	{
	public:
		virtual BOOL HandleEvent(TimeValueType WaitTime) = 0;

		//virtual BOOL Post(DWORD Transferred, DWORD_PTR Key, LPOVERLAPPED Overlapped) = 0;
		virtual BOOL RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler) = 0;
	};

	class WinProactor 
		: public Proactor
	{
	public:
		enum {
			DefaultTimeout = INFINITE
		};

		BOOL Initialize();
		
		BOOL Post(DWORD Transferred, DWORD_PTR Key, LPOVERLAPPED Overlapped);//iocp post
		BOOL RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler);

		//Proactor Pattern Interface
		virtual BOOL HandleEvent(TimeValueType WaitTime = DefaultTimeout);
		
		HANDLE _IOCompletionPort;
	};

	typedef
		Loki::SingletonHolder<WinProactor, Loki::CreateUsingNew, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
			ProactorSingleton
			;

	class AsyncResult;

	class CompletionHandler
	{
	public:
		explicit CompletionHandler() {};
		virtual ~CompletionHandler() {};

		virtual void HandleEvent(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* InOverlapped) = 0; 
		virtual HANDLE GetHandle() = 0;
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