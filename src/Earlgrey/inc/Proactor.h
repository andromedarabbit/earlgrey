#pragma once 

namespace Earlgrey
{
	class NetworkBuffer;

	class IOHandler
	{
		virtual void IODone(BOOL InSuccess, DWORD InTransferred, LPOVERLAPPED InOverlapped) = 0; 
	};

	class Proactor
	{
	public:
		virtual BOOL Post(DWORD Transferred, DWORD_PTR Key, LPOVERLAPPED Overlapped) = 0;
		virtual BOOL Get() = 0;
		virtual BOOL RegisterHandler(HANDLE FileHandle, const void* CompleteHandler) = 0;
	};

	class WinProactor : public Proactor
	{
	public:
		BOOL Initialize();
		
		//Proactor Pattern Interface
		virtual BOOL Post(DWORD Transferred, DWORD_PTR Key, LPOVERLAPPED Overlapped);//iocp post
		virtual BOOL Get();
		virtual BOOL RegisterHandler(HANDLE FileHandle, const void* CompleteHandler);
		
		HANDLE _IOCompletionPort;
	};

	WinProactor GProactor;//! todo
}