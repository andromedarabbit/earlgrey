#pragma once 

namespace Earlgrey
{
	class Socket
	{
	public:
		explicit Socket();
		virtual ~Socket();

		// \note �� �Լ��� �ٸ� Ŭ������ �ִ� ���� ������?
		// \note �ʱ�ȭ�� ������ ���ᵵ �־�� �ϳ�? - WSACleanup
		static BOOL InitializeSubSystem();
		

		virtual void OnConnected();
		virtual void OnDisconnected();

	private:
		SOCKET				_Handle;
		OVERLAPPED			_OverlappedRead;
		OVERLAPPED			_OverlappedSend;
	};
}