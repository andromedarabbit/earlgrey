#include "stdafx.h"

#include "Proactor.h"
#include "EarlgreyMath.h"

namespace Earlgrey
{
	BOOL WinProactor::Initialize()
	{
		// Completion port를 생성한다.
		// 매개변수 NumberOfConcurrentThreads = 0 : 프로세서당 하나의 스레드가 completion port를 처리한다. 
		_IOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if(_IOCompletionPort == NULL)
			return FALSE;
		return TRUE;
	}

	BOOL WinProactor::RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler)
	{
		// Completion port에 특정 핸들을 적용한다. 
		HANDLE ReturnHandle = CreateIoCompletionPort(Handle, _IOCompletionPort, (ULONG_PTR)CompleteHandler, 0);
		if (!ReturnHandle)
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

	BOOL WinProactor::PostEvent(CompletionHandler* CompleteHandler, AsyncResult* ResultStream)
	{
		return PostQueuedCompletionStatus( _IOCompletionPort, 0, (ULONG_PTR)CompleteHandler, ResultStream);
	}

	BOOL WinProactor::HandleEvent(TimeValueType WaitTime)
	{
		DWORD				Transferred = 0;;
		LPOVERLAPPED		Overlapped;
		CompletionHandler*	Handler = NULL;

		BOOL Result = GetQueuedCompletionStatus(_IOCompletionPort, &Transferred, (PULONG_PTR)&Handler, &Overlapped, WaitTime);

		// MSDN 참조
		if (!Result && !Overlapped)
		{
			// timeout!!, not error.
			return TRUE;
		}

		EARLGREY_ASSERT( Overlapped );

		std::auto_ptr<AsyncResult> IOResult( static_cast<AsyncResult*>(Overlapped) );
		IOResult->Status(Result);
		if(!Result)
		{
			IOResult->Error( GetLastError() );
			IOResult->Failed();
		}
		else
		{
			IOResult->TransferredBytes(Transferred);
			IOResult->Completed();
		}

		return Result;
	}


	BOOL AcceptProactor::RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler)
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
		DWORD WaitEventNumber = Math::numeric_cast<DWORD>( _Events.size() );

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
	}
}