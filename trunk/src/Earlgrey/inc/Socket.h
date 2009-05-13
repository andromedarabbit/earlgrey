#pragma once 
#include "Proactor.h"
#include "NetworkBuffer.h"

namespace Earlgrey
{

	class AsyncStream
	{
	public:
		explicit AsyncStream() {};		
		virtual ~AsyncStream() {};

		BOOL Open(SOCKET Socket, CompletionHandler* Handler);
		void Close();

		BOOL AsyncRead(); 
		BOOL AsyncWrite();

	private:
		NetworkBuffer* _PacketBuffer;
		CompletionHandler* _Handler;
		SOCKET _Handle;
	};

	class SocketSubsystem
	{
	public:

		// \note 이 함수는 다른 클래스에 넣는 편이 나을까?
		// \note 초기화가 있으면 종료도 있어야 하나? - WSACleanup
		static BOOL InitializeSubSystem();
		INT GetLastErrorCode()
		{
			return WSAGetLastError();
		}
		const TCHAR* GetSocketError(INT Code = -1);
	
		/*INT GetHostByName(ANSICHAR* HostName, FInternetIpAddr& Addr);
		BOOL GetHostName(String& HostName);
		BOOL GetLocalHostAddr(FInternetIpAddr& HostAddr);*/
	};
}