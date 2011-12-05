#include "stdafx.h"
#include "Proactor.h"

#include "EarlgreyMath.h"
#include "numeric_cast.hpp"
#include "AppSettings.h"
#include "CompletionHandler.h"
#include "NetworkBuffer.h"

#include <sstream>
// #include <memory>


namespace Earlgrey
{
	BOOL Proactor::HandleEvent(TimeSpan WaitTime)
	{
		DWORD milliseconds = 0;
		if(WaitTime == TimeSpan::MaxValue)
			milliseconds = INFINITE; // TODO: INFINITE is just too long, have to new definition EARLGREY_INFINITE
		else
			milliseconds = EARLGREY_NUMERIC_CAST<DWORD>(WaitTime.TotalMilliseconds());

		return HandleEvent(milliseconds);
	}


	WinProactor::WinProactor()
		: _IOCompletionPort(NULL)
	{

	}

	WinProactor::~WinProactor() 
	{
	}

	BOOL WinProactor::Initialize(const DWORD numberOfConcurrentThreads)
	{
		_IOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, numberOfConcurrentThreads);
		if(_IOCompletionPort == NULL)
		{
			// TODO: 오류 메시지는 어떻게 처리할까?
			const DWORD errCode = GetLastError();
			const char * const errMsg = Log::ErrorMessageA(errCode);
			DBG_UNREFERENCED_LOCAL_VARIABLE(errMsg);

			return FALSE;
		}
		return TRUE;
	}

	BOOL WinProactor::Initialize(const AppSettings& appSettings)
	{		
		return Initialize(appSettings.NumberOfConcurrentIOThreads());		
	}

	BOOL WinProactor::RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler)
	{
		EARLGREY_ASSERT(Handle != NULL);
		EARLGREY_ASSERT(CompleteHandler != NULL);

		// Completion port에 특정 핸들을 적용한다. 
		// 매개변수 HANDLE 이 NULL이 아니면 매개변수와 같은 핸들을 반환한다. 그러므로 반환 값을 달리 처리할 필요는 없다.
		HANDLE ReturnHandle = CreateIoCompletionPort(Handle, _IOCompletionPort, (ULONG_PTR)CompleteHandler, 0);
		if (ReturnHandle == NULL) 
		{
			//error
			const DWORD errCode = GetLastError();
			DBG_UNREFERENCED_LOCAL_VARIABLE(errCode);
			return FALSE;
		}

		return TRUE;
	}

	BOOL WinProactor::DeregisterHandler(HANDLE Handle)
	{
		return CloseHandle( Handle );
	}

	BOOL WinProactor::PostEvent(AsyncResult* Result)
	{
		EARLGREY_ASSERT(Result != NULL);
		return PostQueuedCompletionStatus( _IOCompletionPort, 0, (ULONG_PTR)Result->GetHandler(), Result->GetOverlapped() );
	}

	BOOL WinProactor::HandleEvent(TimeSpan WaitTime)
	{
		return __super::HandleEvent(WaitTime);
	}

	BOOL WinProactor::HandleEvent(DWORD milliseconds)
	{
		DWORD				Transferred = 0;;
		LPOVERLAPPED		Overlapped = NULL;
		CompletionHandler*	Handler = NULL;
		
		BOOL Result = GetQueuedCompletionStatus(_IOCompletionPort, &Transferred, (PULONG_PTR)&Handler, &Overlapped, milliseconds);

		// MSDN 참조
		if (!Result && WAIT_TIMEOUT == GetLastError())
		{
			// timeout!!, not error.
			return TRUE;
		}

		EARLGREY_ASSERT( Overlapped );

		// \note AsyncResult 객체는 AsyncStream 객체가 관리하므로 delete 하면 안된다.
		// \note Handler 를 누군가 지우기는 해야 한다.
		std::auto_ptr<AsyncResult> IOResult(
			reinterpret_cast<AsyncResult*>(Overlapped)
			);
		if(!Result)
		{
			IOResult->SetErrorCode( GetLastError() );
			IOResult->SetBytesTransferred( 0 );
		}
		else
		{
			IOResult->SetErrorCode( 0 );
			IOResult->SetBytesTransferred( Transferred );
		}
		
		Handler->HandleEvent( IOResult.get() );

		
		return Result;
	}


	AsyncResult::AsyncResult() 
		: _Handle(INVALID_SOCKET)
		, _Handler(NULL)
	{
		ZeroMemory( &_overlapped, sizeof(_overlapped) );
	}

	AsyncResult::AsyncResult( CompletionHandler* Handler )
		: _Handle(INVALID_SOCKET)
		, _Handler(Handler)
	{
		ZeroMemory( &_overlapped, sizeof(_overlapped) );
	}

	AsyncResult::AsyncResult( SOCKET Handle, CompletionHandler* Handler ) 
		: _Handle(Handle)
		, _Handler(Handler)
	{
		EARLGREY_ASSERT( Handle != INVALID_SOCKET );
		ZeroMemory( &_overlapped, sizeof(_overlapped) );
	}

	AsyncResult::AsyncResult( SOCKET Handle, CompletionHandler* Handler, BufferPtr Buffer ) 
		: _Handle(Handle)
		, _Handler(Handler)
		, _Buffer(Buffer)
	{
		EARLGREY_ASSERT( Handle != INVALID_SOCKET );
		ZeroMemory( &_overlapped, sizeof(_overlapped) );
	}

	AsyncResult::~AsyncResult()
	{
		delete _Handler;
	}

	void AsyncResult::HandleEvent()
	{
		_Handler->HandleEvent( this );
	}

	std::pair<WSABUF*,DWORD> AsyncResult::GetWriteBuffer()
	{
		EARLGREY_ASSERT(_Buffer.get());
		if (!_Buffer.get())
		{
			return std::make_pair<WSABUF*,DWORD>(NULL,0);
		}
		return _Buffer->GetSockSendBuffer();
	}

	void AsyncResult::UpdateSentSize()
	{
		EARLGREY_ASSERT(_Buffer.get());
		_Buffer->OnSent( _BytesTransferred );
		if (_Buffer->SentCompleted())
		{
			_Buffer.reset();
		}
	}

	bool AsyncResult::SentCompleted()
	{
		UpdateSentSize();
		return !_Buffer.get();
	}

	void* AsyncResult::operator new( size_t size )
	{
		UNREFERENCED_PARAMETER(size);
		StlDefaultAllocator<AsyncResult>::Type allocator;
		void* p = allocator.allocate(1);
		if (!p)
		{
			throw std::bad_alloc();
		}
		return p;
	}

	void AsyncResult::operator delete( void* p )
	{
		StlDefaultAllocator<AsyncResult>::Type allocator;
		return allocator.deallocate(reinterpret_cast<AsyncResult*>(p), 0);
	}

}