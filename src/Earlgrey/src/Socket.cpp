#include "stdafx.h"
#include "Socket.h"

#include "txsstream.h"

#pragma comment(lib, "ws2_32.lib")

namespace Earlgrey
{
	// BOOL SocketSubsystem::Initialize()
	namespace 
	{
		BOOL StartupSocket()
		{
			// MSDN 라이브러리의 예제를 참고했다.
			WORD wVersionRequested;
			WSADATA wsaData;
			int err;

			wVersionRequested = MAKEWORD( 2, 2 );

			err = WSAStartup( wVersionRequested, &wsaData );
			if ( err != 0 ) {
				// Tell the user that we could not find a usable Winsock DLL.
				// 소켓 시스템 초기화가 실패했으니 WSAGetLastError 도 동작하지 않는다.
				// \todo 오류 처리
				switch(err)
				{
				case WSASYSNOTREADY: // The underlying network subsystem is not ready for network communication.
					break;

				case WSAVERNOTSUPPORTED: // The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.
					break;

				case WSAEINPROGRESS: // A blocking Windows Sockets 1.1 operation is in progress.
					break;

				case WSAEPROCLIM: // A limit on the number of tasks supported by the Windows Sockets implementation has been reached.
					break;

				case WSAEFAULT: // The lpWSAData parameter is not a valid pointer.
					break;

				default:
					// \todo 오류 메시지를 넣는 ASSERT, VERIFY가 있으면 좋겠다.
					EARLGREY_VERIFY(TRUE);
					break;
				}

				return FALSE;
			}

			/* Confirm that the WinSock DLL supports 2.2.*/
			/* Note that if the DLL supports versions later    */
			/* than 2.2 in addition to 2.2, it will still return */
			/* 2.2 in wVersion since that is the version we      */
			/* requested.                                        */

			if ( LOBYTE( wsaData.wVersion ) != 2 ||
				HIBYTE( wsaData.wVersion ) != 2 ) {
					/* Tell the user that we could not find a usable */
					/* WinSock DLL.                                  */
					WSACleanup( );
					return FALSE; 
			}

			return TRUE;
		}

		BOOL CleanupSocket()
		{
			if(::WSACleanup() == SOCKET_ERROR)
			{
				// WSAGetLastError 
				return FALSE;
			}
			return TRUE;
		}
	}



	BOOL Socket::s_Initialized = FALSE;
	// BOOL Socket::s_UseOverlappedIO = FALSE;

	Socket::mutex_type Socket::s_InternalSyncObject;

	void Socket::InitializeSockets()
	{
		if(s_Initialized)
			return;

		scoped_lock_type scopeLock(s_InternalSyncObject);

		if(s_Initialized)
			return;

		if(StartupSocket())
			s_Initialized = TRUE;


		// Environment::Is_Win_Server()
	}

	void Socket::UninitializeSockets()
	{
		if(CleanupSocket())
		{
			s_Initialized = FALSE;
			return;
		}
	}




	bool Socket::CreateTcpSocket()
	{
		// InitializeSockets();

		_Handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(_Handle != INVALID_SOCKET)
			return true;

		// TODO: 임시코드
		const DWORD errCode = WSAGetLastError();
		const _txstring msg = Log::ErrorMessage(errCode);
		DBG_UNREFERENCED_LOCAL_VARIABLE(msg);
		return false;
	}

	bool Socket::CreateAsyncTcpSocket()
	{
		_Handle = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
		return _Handle != INVALID_SOCKET;
	}

	bool Socket::Bind(const IPEndPoint& localEP)
	{
		EARLGREY_ASSERT(IsValid() == false);

		ADDRINFOT aiHints = { 0 };
		aiHints.ai_family = AF_UNSPEC;
		aiHints.ai_socktype = SOCK_STREAM;
		aiHints.ai_protocol = IPPROTO_TCP;
		aiHints.ai_flags = AI_PASSIVE;

		ADDRINFOT * aiList = NULL;
		handle_t regKeyHandle(aiList, &FreeAddrInfo);

		Socket::InitializeSockets(); // GetAddrInfo 호출 전에...

		// TODO: 임시
		_txstringstream ss;
		ss << localEP.Port();


		const int retVal = ::GetAddrInfo(localEP.Address().ToString().c_str(), ss.str().c_str(), &aiHints, &aiList);		
		if (retVal != 0) {
			return false;
		}

		ADDRINFOT * current = aiList;
		do
		{
			SOCKET handle = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
			if(handle == INVALID_SOCKET)
				continue;

			if(bind(handle, current->ai_addr, EARLGREY_NUMERIC_CAST<int>(current->ai_addrlen)) == SOCKET_ERROR)
				continue;

			_Handle = handle;
			EARLGREY_ASSERT(IsValid());

			return true;
		} while ( NULL != (current = current->ai_next) );

		return false;
	}

}