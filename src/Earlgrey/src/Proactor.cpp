#include "stdafx.h"

#include "Proactor.h"
#include "EarlgreyMath.h"

namespace Earlgrey
{
	BOOL Proactor::HandleEvent(TimeSpan WaitTime)
	{
		DWORD milliseconds = 0;
		if(WaitTime == TimeSpan::MaxValue)
			milliseconds = INFINITE;
		else
			milliseconds = static_cast<DWORD>(WaitTime.TotalMilliseconds());

		return HandleEvent(milliseconds);
	}



	BOOL WinProactor::Initialize()
	{
		// Completion port�� �����Ѵ�.
		// �Ű����� NumberOfConcurrentThreads = 0 : ���μ����� �ϳ��� �����尡 completion port�� ó���Ѵ�. 
		_IOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if(_IOCompletionPort == NULL) //TODO: ���� �ڵ�� Ȯ�� �� �ϳ�?
			return FALSE;
		return TRUE;
	}

	BOOL WinProactor::RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler)
	{
		EARLGREY_ASSERT(Handle != NULL);
		EARLGREY_ASSERT(CompleteHandler != NULL);

		// Completion port�� Ư�� �ڵ��� �����Ѵ�. 
		// TODO: ��ȯ ���� �ڵ��� �ƹ�¦�� ���� ����? ���� ó�� �ܿ�?
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
		LPOVERLAPPED		Overlapped;
		CompletionHandler*	Handler = NULL;
		
		BOOL Result = GetQueuedCompletionStatus(_IOCompletionPort, &Transferred, (PULONG_PTR)&Handler, &Overlapped, milliseconds);

		// MSDN ����
		if (!Result && WAIT_TIMEOUT == GetLastError())
		{
			// timeout!!, not error.
			return TRUE;
		}

		EARLGREY_ASSERT( Overlapped );

		// AsyncResult ��ü�� AsyncStream ��ü�� �����ϹǷ� delete �ϸ� �ȵȴ�.
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
}