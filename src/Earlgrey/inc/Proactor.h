#pragma once 

#include <Loki/Singleton.h>
#include <Loki/Threads.h> // for Loki::SingleThreaded
#include "NoLock.h"

namespace Earlgrey
{
	class NetworkBuffer;

	class IOHandler
	{
	public:
		virtual void IODone(BOOL InSuccess, DWORD InTransferred, LPOVERLAPPED InOverlapped) = 0; 
	};

	class Proactor
	{
	public:
		virtual BOOL Post(DWORD Transferred, DWORD_PTR Key, LPOVERLAPPED Overlapped) = 0;
		virtual BOOL Dispatcher() = 0;
		virtual BOOL RegisterHandler(HANDLE FileHandle, const void* CompleteHandler) = 0;
	};

	class WinProactor : public Proactor
	{
	public:
		BOOL Initialize();
		
		//Proactor Pattern Interface
		virtual BOOL Post(DWORD Transferred, DWORD_PTR Key, LPOVERLAPPED Overlapped);//iocp post
		virtual BOOL Dispatcher();
		virtual BOOL RegisterHandler(HANDLE FileHandle, const void* CompleteHandler);
		
		HANDLE _IOCompletionPort;
	};

	typedef
		Loki::SingletonHolder<WinProactor, Loki::CreateStatic, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
			ProactorSingleton
			;
}