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

	//Windows IOCompletion Port ��
	class Proactor : private Uncopyable
	{
	public:
		explicit Proactor() {}
		virtual ~Proactor() {}

		// virtual BOOL HandleEvent(TimeValueType WaitTime) = 0;
		virtual BOOL HandleEvent(TimeSpan WaitTime);
		virtual BOOL HandleEvent(DWORD milliseconds) = 0;

		//! TODO: �̷��� �ü�� �ڵ��� �巯������ �ְ� �޾Ƶ� �ǳ�?  
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

	//! Async I/O ó���� ���� �Ѱ����� token
	/*!
		Read / Write ���� �������� ����ϴ� token�̴�.
		�Ϸ�Ǹ� �� token�� ������ ������ ä���� �� �ڵ鷯���� �Ѱ�����.
	*/
	class AsyncResult
	{
		friend class WinProactor;
	public:
		AsyncResult()
		{
		}

		//! I/O �� ��� ���� ��ü�� ť�� ���� �� ����ϴ� ������
		AsyncResult(CompletionHandler* Handler)
			: _Handle(INVALID_SOCKET), _Handler(Handler)
		{
		}

		//! I/O ó���� ����ϴ� ������
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

		//! �ڵ鷯�� ȣ���Ѵ�. Receiver / Sender ..
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