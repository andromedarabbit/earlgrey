#include "stdafx.h"

#include "ServerInit.h"
#include "Socket.h"
#include "ScopedLock.h"

#pragma comment(lib, "ws2_32.lib")

namespace Earlgrey
{
	// BOOL SocketSubsystem::Initialize()
	namespace 
	{
		BOOL StartupSocket()
		{
			// MSDN ���̺귯���� ������ �����ߴ�.
			WORD wVersionRequested;
			WSADATA wsaData;
			int err;

			wVersionRequested = MAKEWORD( 2, 2 );

			err = WSAStartup( wVersionRequested, &wsaData );
			if ( err != 0 ) {
				// Tell the user that we could not find a usable Winsock DLL.
				// ���� �ý��� �ʱ�ȭ�� ���������� WSAGetLastError �� �������� �ʴ´�.
				// \todo ���� ó��
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
					// \todo ���� �޽����� �ִ� ASSERT, VERIFY�� ������ ���ڴ�.
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

	Loki::Mutex Socket::s_InternalSyncObject;

	void Socket::InitializeSockets()
	{
		if(s_Initialized)
			return;

		ScopedLock scopeLock(&s_InternalSyncObject);

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
}