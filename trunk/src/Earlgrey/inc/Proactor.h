#pragma once 

#include <Loki/Singleton.h>
#include <Loki/Threads.h> // for Loki::SingleThreaded
#include <vector>
#include <map>

#include "NoLock.h"
#include "TimeSpan.h"

namespace Earlgrey
{
	class NetworkBuffer;
	class AsyncResult;
	class CompletionHandler;
	class AppSettings;

	//Windows IOCompletion Port 용
	class Proactor : private Uncopyable
	{
	public:
		explicit Proactor() {}
		virtual ~Proactor() {}

		// virtual BOOL HandleEvent(TimeValueType WaitTime) = 0;
		virtual BOOL HandleEvent(TimeSpan WaitTime);
		virtual BOOL HandleEvent(DWORD milliseconds) = 0;

		//! TODO: 이렇게 운영체제 핸들을 드러내놓고 주고 받아도 되나?  
		virtual BOOL RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler) = 0;
		virtual BOOL DeregisterHandler(HANDLE Handle) = 0;
	};

	class WinProactor 
		: public Proactor
	{
	public:
		explicit WinProactor();		
		virtual ~WinProactor();

		BOOL Initialize(const AppSettings& appSettings);
		
		// Proactor Pattern Interface
		virtual BOOL HandleEvent(TimeSpan WaitTime = TimeSpan::MaxValue);
		virtual BOOL HandleEvent(DWORD WaitTime = INFINITE); 
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

	class CompletionHandler : private Uncopyable
	{
	public:
		explicit CompletionHandler() {};
		virtual ~CompletionHandler() {};

		virtual void HandleEvent(AsyncResult* Result) = 0;
	};

	//! Async I/O 처리를 위해 넘겨지는 token
	/*!
		Read / Write 에서 공통으로 사용하는 token이다.
		완료되면 이 token이 적당한 값으로 채워진 후 핸들러에게 넘겨진다.
	*/
	class AsyncResult
	{
		friend class WinProactor;
	public:
		AsyncResult()
		{
		}

		//! I/O 와 상관 없는 객체를 큐에 넣을 때 사용하는 생성자
		AsyncResult(CompletionHandler* Handler)
			: _Handle(INVALID_SOCKET), _Handler(Handler)
		{
		}

		//! I/O 처리에 사용하는 생성자
		AsyncResult(SOCKET Handle, CompletionHandler* Handler) 
			: _Handle(Handle), _Handler(Handler)
		{
			EARLGREY_ASSERT( Handle != INVALID_SOCKET );
		}

		DWORD GetBytesTransferred() const
		{
			return _BytesTransferred;
		}

		SOCKET GetHandle() const
		{
			return _Handle;
		}

		//! 핸들러를 호출한다. Receiver / Sender ..
		void HandleEvent()
		{
			_Handler->HandleEvent( this );
		}

		CompletionHandler* GetHandler()
		{
			return _Handler;
		}

		OVERLAPPED* GetOverlapped()
		{
			return &_overlapped;
		}

		DWORD GetError() const
		{
			return _Error;
		}

	private:
		void SetBytesTransferred(DWORD Bytes)
		{
			_BytesTransferred = Bytes;
		}

		void SetErrorCode(DWORD Error)
		{
			_Error = Error;
		}

	private:
		OVERLAPPED _overlapped;
		CompletionHandler* _Handler;
		DWORD _BytesTransferred;
		SOCKET _Handle;
		DWORD _Error;
	};

}