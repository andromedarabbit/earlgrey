#include "stdafx.h"
#include "Proactor.h"

#include "EarlgreyMath.h"
#include "numeric_cast.hpp"
#include "AppSettings.h"
#include "CompletionHandler.h"
#include <sstream>
#include "NetworkBuffer.h"

namespace Earlgrey
{
	BOOL Proactor::HandleEvent(TimeSpan WaitTime)
	{
		DWORD milliseconds = 0;
		if(WaitTime == TimeSpan::MaxValue)
			milliseconds = INFINITE;
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

	BOOL WinProactor::Initialize(const AppSettings& appSettings)
	{
		// Completion port를 생성한다.
		_IOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, appSettings.NumberOfConcurrentIOThreads());
		if(_IOCompletionPort == NULL) //TODO: 오류 코드는 확인 안 하나?
			return FALSE;
		return TRUE;
	}

	BOOL WinProactor::RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler)
	{
		EARLGREY_ASSERT(Handle != NULL);
		EARLGREY_ASSERT(CompleteHandler != NULL);

		// Completion port에 특정 핸들을 적용한다. 
		// TODO: 반환 받은 핸들은 아무짝에 쓸모가 없나? 오류 처리 외엔?
		HANDLE ReturnHandle = CreateIoCompletionPort(Handle, _IOCompletionPort, (ULONG_PTR)CompleteHandler, 0);
		if (ReturnHandle == NULL) 
		{
			//error
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

		// AsyncResult 객체는 AsyncStream 객체가 관리하므로 delete 하면 안된다.
		AsyncResult* IOResult = reinterpret_cast<AsyncResult*>(Overlapped);
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
		
		Handler->HandleEvent( IOResult );

		return Result;
	}


	/*BOOL AcceptProactor::RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler)
	{
		//lock? serializer?
		EnterCriticalSection(&Lock_);

		_Events.push_back((WSAEVENT)Handle);
		_EventHandlerMap.insert( std::make_pair((WSAEVENT)Handle, static_cast<WaitEventHandler*>(CompleteHandler)) );

		LeaveCriticalSection(&Lock_);
		return TRUE;
	}

	BOOL AcceptProactor::DeregisterHandler(HANDLE Handle)
	{
		
		EnterCriticalSection(&Lock_);
		EventVectorType::iterator i = std::find( _Events.begin(), _Events.end(), (WSAEVENT)Handle );
		if (i != _Events.end()) 
		{
			_Events.erase( i );
		}
		_EventHandlerMap.erase( (WSAEVENT)Handle );
		LeaveCriticalSection(&Lock_);

		return TRUE;
	}

	BOOL AcceptProactor::HandleEvent(TimeValueType WaitTime)
	{
		BOOL Result = FALSE;
		DWORD WaitEventNumber = EARLGREY_NUMERIC_CAST<DWORD>( _Events.size() );

		if (WaitEventNumber == 0)
		{
			Sleep(WaitTime);			
			return TRUE;
		}

		EnterCriticalSection(&Lock_);

		DWORD Index	= WaitForMultipleObjects(
			WaitEventNumber,
			&_Events[0],
			FALSE,
			WaitTime
			);

		if (WAIT_OBJECT_0 <= Index &&
			Index < WAIT_OBJECT_0 + WaitEventNumber)
		{
			EventHandlerMapType::const_iterator i = _EventHandlerMap.find( _Events[Index] );
			EARLGREY_ASSERT( i != _EventHandlerMap.end() );
			if (i != _EventHandlerMap.end())
			{
				i->second->HandleEvent();
			}			
			Result = TRUE;
		}
		else if (Index == WAIT_TIMEOUT)
		{
			Result = TRUE;
		}
		else
		{
		}

		LeaveCriticalSection(&Lock_);

		return Result;
	}*/

	void AsyncResult::HandleEvent()
	{
		_Handler->HandleEvent( this );
	}

	AsyncResult::AsyncResult() : _Handle(INVALID_SOCKET), _Handler(NULL)
	{
		ZeroMemory( &_overlapped, sizeof(_overlapped) );
	}

	AsyncResult::AsyncResult( CompletionHandler* Handler ) : _Handle(INVALID_SOCKET), _Handler(Handler)
	{
		ZeroMemory( &_overlapped, sizeof(_overlapped) );
	}

	AsyncResult::AsyncResult( SOCKET Handle, CompletionHandler* Handler ) : _Handle(Handle), _Handler(Handler)
	{
		EARLGREY_ASSERT( Handle != INVALID_SOCKET );
		ZeroMemory( &_overlapped, sizeof(_overlapped) );
	}

	AsyncResult::AsyncResult( SOCKET Handle, CompletionHandler* Handler, BufferPtr Buffer ) : _Handle(Handle), _Handler(Handler), _Buffer(Buffer)
	{
		EARLGREY_ASSERT( Handle != INVALID_SOCKET );
		ZeroMemory( &_overlapped, sizeof(_overlapped) );
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