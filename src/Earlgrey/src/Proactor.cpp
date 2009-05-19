#include "stdafx.h"
#include "Proactor.h"

namespace Earlgrey
{
	BOOL WinProactor::Initialize()
	{
		_IOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		return TRUE;
	}

	BOOL WinProactor::RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler)
	{
		HANDLE ReturnHandle = CreateIoCompletionPort(Handle, _IOCompletionPort, (ULONG_PTR)CompleteHandler, 0);
		if (!ReturnHandle)
		{
			//error
			return FALSE;
		}
		return TRUE;
	}

	BOOL WinProactor::DeregisterHandler(HANDLE /*Handle*/)
	{
		return TRUE;
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

		// MSDN ÂüÁ¶
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

		Events.push_back((WSAEVENT)Handle);
		EventHandlers.push_back(static_cast<WaitEventHandler*>(CompleteHandler));
		return TRUE;
	}

	BOOL AcceptProactor::DeregisterHandler(HANDLE Handle)
	{
		//lock? serializer?
		for(DWORD i = 0; i < Events.size(); i++)
		{
			if(Events[i] == Handle)
			{
				Events.erase( Events.begin() + i);
				EventHandlers.erase(EventHandlers.begin() + i);
			}
		}

		return TRUE;
	}

	BOOL AcceptProactor::HandleEvent(TimeValueType WaitTime)
	{
		BOOL Result = FALSE;
		DWORD WaitEventNumber = (DWORD)Events.size();

		DWORD Index	= WaitForMultipleObjects(
			WaitEventNumber,
			&Events[0],
			FALSE,
			WaitTime
			);

		if (WAIT_OBJECT_0 <= Index &&
			Index < WAIT_OBJECT_0 + WaitEventNumber)
		{
			EventHandlers[Index]->HandleEvent();
			Result = TRUE;
		}
		else if (Index == WAIT_TIMEOUT)
		{		
		}
		else
		{
		}

		return Result;
	}
}