#pragma once 
#include "Proactor.h"
#include "NetworkBuffer.h"

namespace Earlgrey
{
	class SocketInterface : public IOHandler
	{
	public:
		explicit SocketInterface(SOCKET Socket)
		{
			_Handle = Socket;
			Initialize();
		};

		virtual ~SocketInterface();
		
		BOOL Initialize();
		void Close();
		BOOL Send();
		BOOL Receive();

		//SocketInterface
		virtual void OnConnected();
		virtual void OnDisconnected();
		virtual void OnReceived(DWORD InTransferred);

		//IOHandler
		virtual void IODone(BOOL InSuccess, DWORD InTransferred, LPOVERLAPPED InOverlapped);

	private:
		void ReceiveCompleted(DWORD InTransferred);
		void SendCompleted(DWORD InTransferred);

		SOCKET _Handle;
		OVERLAPPED _OverlappedRead;
		OVERLAPPED _OverlappedSend;

		NetworkBuffer* _PacketBuffer;
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