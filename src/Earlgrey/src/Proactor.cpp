#include "stdafx.h"
#include "Proactor.h"

namespace Earlgrey
{
	BOOL WinProactor::Initialize()
	{
		_IOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,	NULL, 0, 0);
		return TRUE;
	}

	BOOL WinProactor::RegisterHandler(HANDLE FileHandle, const void* CompleteHandler)
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

	BOOL WinProactor::Get()
	{
		BOOL			WaitMilliSecond = 50;
		DWORD			Transferred = 0;;
		LPOVERLAPPED	Overlapped;
		ULONG_PTR		Handler = 0;

		BOOL IOSuccess = GetQueuedCompletionStatus( 
								_IOCompletionPort, 
								&Transferred,
								(PULONG_PTR)&Handler,
								&Overlapped,
								WaitMilliSecond
								);

		if (!Handler || !IOSuccess)
		{
			//error Ãâ·Â
			return FALSE;
		}

		//(static_cast<IOHandler*>(Handler))->IODone(IOSuccess, Transferred, Overlapped);

		return TRUE;
	}
}