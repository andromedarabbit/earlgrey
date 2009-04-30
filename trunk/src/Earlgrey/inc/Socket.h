#pragma once 

namespace Earlgrey
{
	class Socket
	{
	public:
		explicit Socket();
		virtual ~Socket();

		// \note 이 함수는 다른 클래스에 넣는 편이 나을까?
		// \note 초기화가 있으면 종료도 있어야 하나? - WSACleanup
		static BOOL InitializeSubSystem();
		

		virtual void OnConnected();
		virtual void OnDisconnected();

	private:
		SOCKET				_Handle;
		OVERLAPPED			_OverlappedRead;
		OVERLAPPED			_OverlappedSend;
	};
}