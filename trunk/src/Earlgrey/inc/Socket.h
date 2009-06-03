#pragma once 

namespace Earlgrey
{
	class CompletionHandler;
	class NetworkBuffer;
	class AsyncStream
	{
	public:
		explicit AsyncStream() {};		
		virtual ~AsyncStream() {};

		BOOL Open(SOCKET Socket, CompletionHandler* Handler);
		void Close();

		BOOL Post();
		BOOL AsyncRead(); 
		BOOL AsyncWrite();

		NetworkBuffer* GetNetworkBuffer() { return _PacketBuffer; }
	private:
		NetworkBuffer* _PacketBuffer;
		CompletionHandler* _Handler;
		SOCKET _Handle;
	};

	class SocketSubsystem
	{
	public:

		// \note �� �Լ��� �ٸ� Ŭ������ �ִ� ���� ������?
		// \note �ʱ�ȭ�� ������ ���ᵵ �־�� �ϳ�? - WSACleanup
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