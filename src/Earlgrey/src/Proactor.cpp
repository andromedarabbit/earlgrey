#include "stdafx.h"
#include "Proactor.h"

namespace Earlgrey
{
	BOOL WinProactor::Initialize()
	{
		_IOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		return TRUE;
	}

	BOOL WinProactor::RegisterHandler(HANDLE FileHandle, CompletionHandler* CompleteHandler)
	{
		HANDLE ReturnHandle = CreateIoCompletionPort(FileHandle, _IOCompletionPort, (ULONG_PTR)CompleteHandler, 0);
		if (!ReturnHandle)
		{
			//error
			return FALSE;
		}
		return TRUE;
	}

	BOOL WinProactor::Post(DWORD Transferred, DWORD_PTR Key, LPOVERLAPPED Overlapped)
	{
		return PostQueuedCompletionStatus( _IOCompletionPort, Transferred, Key, Overlapped );
	}

	BOOL WinProactor::Dispatcher()
	{
	
		BOOL			WaitMilliSecond = 50;
		DWORD			Transferred = 0;;
		LPOVERLAPPED	Overlapped;
		CompletionHandler*		Handler = 0;//handler 바로 호출하면 안되나???

		BOOL Result = GetQueuedCompletionStatus(_IOCompletionPort, &Transferred, (PULONG_PTR)&Handler, &Overlapped, WaitMilliSecond);

		//EARLGREY_ASSERT(Overlapped);
		if (!Handler || !Overlapped)
		{
			//error 출력
			return FALSE;
		}

		AsyncResult* IOResult = static_cast<AsyncResult*>(Overlapped);
		IOResult->Status(Result);
		if(!Result)
		{
			IOResult->Failed();
		}
		else
		{
			IOResult->TransferredBytes(Transferred);
			IOResult->Completed();
		}

		delete IOResult;

		return TRUE;
	}
}