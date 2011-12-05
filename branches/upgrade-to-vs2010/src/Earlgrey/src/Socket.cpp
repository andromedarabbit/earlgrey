#include "stdafx.h"
#include "Socket.h"


#include "IPAddress.h"
#include "IPEndPoint.h"

#include "Log.h"
#include "numeric_cast.hpp"
#include "RAII.h"
#include "txsstream.h"

#pragma warning( push ) 
#pragma warning( disable : 4996 )
#include <ws2tcpip.h>
#pragma warning( pop ) 

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

		SOCKET CreateWSASocket(AddressFamily::E addressFamily, SocketType::E socketType, ProtocolType::E protocolType)
		{
			SOCKET handle = socket(addressFamily, socketType, protocolType);
			if(handle != INVALID_SOCKET)
				return true;

			// TODO: �ӽ��ڵ�
			const DWORD errCode = WSAGetLastError();
			const CHAR * const errMsg = Log::ErrorMessageA(errCode);
			throw std::exception(errMsg);
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


	Socket::Socket()
		: m_Handle(INVALID_SOCKET)
		, m_AddressFamily(AddressFamily::InterNetwork)
		, m_SocketType(SocketType::Stream)
		, m_ProtocolType(ProtocolType::Tcp)
		, m_IsListening(FALSE)
		, m_EndPointBound(NULL)
	{
		Socket::InitializeSockets();

		m_Handle = CreateWSASocket(m_AddressFamily, m_SocketType, m_ProtocolType);
	}


	Socket::Socket(AddressFamily::E addressFamily, SocketType::E socketType, ProtocolType::E protocolType)
		: m_Handle(INVALID_SOCKET)
		, m_AddressFamily(addressFamily)
		, m_SocketType(socketType)
		, m_ProtocolType(protocolType)
		, m_IsListening(FALSE)
		, m_EndPointBound(NULL)
	{
		Socket::InitializeSockets();

		m_Handle = CreateWSASocket(m_AddressFamily, m_SocketType, m_ProtocolType);
	}

	Socket::~Socket()
	{
		closesocket(m_Handle);
		delete m_EndPointBound;
	}

	void Socket::Bind(const IPEndPoint& localEP)
	{			
		/*const IPAddress2& address = localEP.Address();
		const SOCKADDR_STORAGE & rawAddress = address.AddressStorage();
	
		if(bind(m_Handle, (const struct sockaddr*) &rawAddress, sizeof(rawAddress)) == SOCKET_ERROR)
		{
			const DWORD errCode = WSAGetLastError();
			const CHAR * const msg = Log::ErrorMessageA(errCode);
			throw std::exception(msg);
		}

		m_EndPointBound = new IPEndPoint(localEP.Address(), localEP.Port());*/

		// EARLGREY_ASSERT(IsValid() == false);

		ADDRINFOT aiHints = { 0 };
		aiHints.ai_family = AF_UNSPEC;
		aiHints.ai_socktype = SOCK_STREAM;
		aiHints.ai_protocol = IPPROTO_TCP;
		aiHints.ai_flags = AI_PASSIVE;

		ADDRINFOT * aiList = NULL;
		handle_t regKeyHandle(aiList, &FreeAddrInfo);

		Socket::InitializeSockets(); // GetAddrInfo ȣ�� ����...

		_txstringstream ss;
		ss << localEP.Port();

		const int retVal = ::GetAddrInfo(localEP.Address().ToString().c_str(), ss.str().c_str(), &aiHints, &aiList);		
		if (retVal != 0) {
			const DWORD errCode = WSAGetLastError();
			const CHAR * const msg = Log::ErrorMessageA(errCode);
			throw std::exception(msg);
		}

		ADDRINFOT * current = aiList;
		do
		{
			SOCKET handle = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
			if(handle == INVALID_SOCKET)
				continue;

			if(bind(handle, current->ai_addr, EARLGREY_NUMERIC_CAST<int>(current->ai_addrlen)) == SOCKET_ERROR)
				continue;

			m_Handle = handle;
			// EARLGREY_ASSERT(IsValid());

			m_EndPointBound = new IPEndPoint(localEP.Address(), localEP.Port());
			return;
		} while ( NULL != (current = current->ai_next) );

		throw std::exception();
	}

	void Socket::Listen(int MaxConnections)
	{
		if( listen(m_Handle, MaxConnections) == SOCKET_ERROR )
		{				
			const DWORD errCode = WSAGetLastError();
			const CHAR * const msg = Log::ErrorMessageA(errCode);
			throw std::exception(msg);
		}
		m_IsListening = TRUE;
	}

	BOOL Socket::IsBound() const
	{
		return m_EndPointBound != NULL;
	}

	// TODO: �ڵ��� �Ѱ��ִ� �޼��带 �����ص� �Ǵ� �ǰ�?
	SOCKET Socket::GetHandle() const
	{
		return m_Handle;
	}

	// TODO: �ɼ� ���� �ߺ� �ڵ带 ��� ��������.
	void Socket::ExclusiveAddressUse(BOOL use) const
	{
		if (m_Handle == INVALID_SOCKET)
		{
			throw std::exception("Socket is not valid!");
		}

		if (IsBound() == TRUE)
		{
			throw std::exception("Socket should not be bound!");
		}

		const BOOL optionValue = use;
		if(setsockopt(m_Handle, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<const char*>(&optionValue), sizeof(optionValue)) == SOCKET_ERROR)
		{
			const DWORD errCode = WSAGetLastError();
			const CHAR * const msg = Log::ErrorMessageA(errCode);
			throw std::exception(msg);
		}
	}

	BOOL Socket::ExclusiveAddressUse() const
	{
		BOOL optionValue = TRUE;
		int optionLength = 0;
		if(getsockopt(m_Handle, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<char*>(&optionValue), &optionLength) == SOCKET_ERROR)
		{
			const DWORD errCode = WSAGetLastError();
			const CHAR * const msg = Log::ErrorMessageA(errCode);
			throw std::exception(msg);
		}
		return optionValue;
	}

	void Socket::ReuseAddress(BOOL reuse) const
	{
		if (m_Handle == INVALID_SOCKET)
		{
			throw std::exception("Socket is not valid!");
		}

		if (IsBound() == TRUE)
		{
			throw std::exception("Socket should not be bound!");
		}

		const BOOL optionValue = reuse;
		if(setsockopt(m_Handle, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&optionValue), sizeof(optionValue)) == SOCKET_ERROR)
		{
			const DWORD errCode = WSAGetLastError();
			const CHAR * const msg = Log::ErrorMessageA(errCode);
			throw std::exception(msg);
		}
	}

	BOOL Socket::ReuseAddress() const
	{
		BOOL optionValue = TRUE;
		int optionLength = 0;
		if(getsockopt(m_Handle, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&optionValue), &optionLength) == SOCKET_ERROR)
		{
			const DWORD errCode = WSAGetLastError();
			const CHAR * const msg = Log::ErrorMessageA(errCode);
			throw std::exception(msg);
		}
		return optionValue;
	}



	//bool Socket::CreateTcpSocket()
	//{
	//	// InitializeSockets();

	//	_Handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//	if(_Handle != INVALID_SOCKET)
	//		return true;

	//	// TODO: �ӽ��ڵ�
	//	const DWORD errCode = WSAGetLastError();
	//	const _txstring msg = Log::ErrorMessage(errCode);
	//	DBG_UNREFERENCED_LOCAL_VARIABLE(msg);
	//	return false;
	//}

	//bool Socket::CreateAsyncTcpSocket()
	//{
	//	_Handle = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	//	return _Handle != INVALID_SOCKET;
	//}

	//bool Socket::Bind(const IPEndPoint& localEP)
	//{
	//	EARLGREY_ASSERT(IsValid() == false);

	//	ADDRINFOT aiHints = { 0 };
	//	aiHints.ai_family = AF_UNSPEC;
	//	aiHints.ai_socktype = SOCK_STREAM;
	//	aiHints.ai_protocol = IPPROTO_TCP;
	//	aiHints.ai_flags = AI_PASSIVE;

	//	ADDRINFOT * aiList = NULL;
	//	handle_t regKeyHandle(aiList, &FreeAddrInfo);

	//	Socket2::InitializeSockets(); // GetAddrInfo ȣ�� ����...

	//	// TODO: �ӽ�
	//	_txstringstream ss;
	//	ss << localEP.Port();


	//	const int retVal = ::GetAddrInfo(localEP.Address().ToString().c_str(), ss.str().c_str(), &aiHints, &aiList);		
	//	if (retVal != 0) {
	//		return false;
	//	}

	//	ADDRINFOT * current = aiList;
	//	do
	//	{
	//		SOCKET handle = socket(current->ai_family, current->ai_socktype, current->ai_protocol);
	//		if(handle == INVALID_SOCKET)
	//			continue;

	//		if(bind(handle, current->ai_addr, EARLGREY_NUMERIC_CAST<int>(current->ai_addrlen)) == SOCKET_ERROR)
	//			continue;

	//		_Handle = handle;
	//		EARLGREY_ASSERT(IsValid());

	//		return true;
	//	} while ( NULL != (current = current->ai_next) );

	//	return false;
	//}

}