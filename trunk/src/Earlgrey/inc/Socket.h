#pragma once 
#include "Log.h"
#include <Loki/Threads.h>
#include <winsock2.h>

namespace Earlgrey
{
	//class SocketSubsystem
	//{
	//public:

	//	 \note 이 함수는 다른 클래스에 넣는 편이 나을까?
	//	 \note 초기화가 있으면 종료도 있어야 하나? - WSACleanup
	//	static BOOL Initialize();
 //		INT GetLastErrorCode()
 //		{
 //			return WSAGetLastError();
 //		}
	//	const TCHAR* GetSocketError(INT Code = -1);
	//
	//	/*INT GetHostByName(ANSICHAR* HostName, FInternetIpAddr& Addr);
	//	BOOL GetHostName(String& HostName);
	//	BOOL GetLocalHostAddr(FInternetIpAddr& HostAddr);*/
	//};


	class Socket : private Uncopyable
	{
	public:
		typedef Loki::Mutex mutex_type;
		typedef ScopedLock<mutex_type> scoped_lock_type;

		explicit Socket() 
			: _Handle(INVALID_SOCKET)
		{
			Socket::InitializeSockets();
		}

		explicit Socket(SOCKET handle)
			: _Handle(handle)
		{

		}

// 		SOCKET Attach(SOCKET Handle)
// 		{
// 			std::swap( Handle, _Handle );
// 			return Handle;
// 		}

		bool CreateTcpSocket()
		{
			_Handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if(_Handle != INVALID_SOCKET)
				return true;

			// TODO: 임시코드
			const DWORD errCode = WSAGetLastError();
			const _txstring msg = Log::ErrorMessage(errCode);
			DBG_UNREFERENCED_LOCAL_VARIABLE(msg);
			return false;
		}

		bool CreateAsyncTcpSocket()
		{
			_Handle = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
			return _Handle != INVALID_SOCKET;
		}

		void Close()
		{
			if (INVALID_SOCKET != _Handle)
			{
				closesocket( _Handle );
			}
		}

		bool Bind(USHORT Port)
		{
			SOCKADDR_IN Address;
			Address.sin_family = AF_INET;
			Address.sin_addr.s_addr = htonl(INADDR_ANY);
			Address.sin_port = htons((u_short)Port);

			if(bind(_Handle, (const struct sockaddr*) &Address, sizeof(Address)) != SOCKET_ERROR)
				return true;

			// TODO: 임시코드
			const DWORD errCode = WSAGetLastError();
			const _txstring msg = Log::ErrorMessage(errCode);
			DBG_UNREFERENCED_LOCAL_VARIABLE(msg);
			return false;

		}

		bool Listen(int MaxConnections = SOMAXCONN)
		{
			return listen(_Handle, MaxConnections) != SOCKET_ERROR;
		}

		bool SetReceiveBufferSize(INT Size) const
		{
			EARLGREY_ASSERT(Size >= 0);
			return SOCKET_ERROR != setsockopt( _Handle, SOL_SOCKET, SO_RCVBUF, (const char*)&Size, sizeof(Size) );
		}

		bool SetSendBufferSize(INT Size) const
		{
			EARLGREY_ASSERT(Size >= 0);
			return SOCKET_ERROR != setsockopt( _Handle, SOL_SOCKET, SO_SNDBUF, (const char*)&Size, sizeof(Size) );
		}

		SOCKET GetHandle() const
		{
			return _Handle;
		}

		bool SetReuseAddress() const
		{
			EARLGREY_ASSERT( _Handle != INVALID_SOCKET );
			if (INVALID_SOCKET == _Handle)
			{
				return false;
			}
			BOOL Value = TRUE;
			return SOCKET_ERROR != setsockopt(_Handle, SOL_SOCKET, SO_REUSEADDR, (const char*)&Value, sizeof(Value));
		}

		bool SetExclusiveAddressUse() const
		{
			EARLGREY_ASSERT( _Handle != INVALID_SOCKET );
			if (INVALID_SOCKET == _Handle)
			{
				return false;
			}
			BOOL Value = TRUE;
			return SOCKET_ERROR != setsockopt( _Handle, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (const char*)&Value, sizeof(Value));
		}

		bool SetNonBlockingSocket() const 
		{
			DWORD temp = 1;
			return (ioctlsocket( _Handle, FIONBIO, &temp ) != SOCKET_ERROR);
		}

		bool IsValid() const
		{
			return _Handle != INVALID_SOCKET;
		}

	// private:
		static void InitializeSockets();
		static void UninitializeSockets();


	private:
		static BOOL s_Initialized;
		// static BOOL s_UseOverlappedIO;
		static Loki::Mutex s_InternalSyncObject;

		SOCKET _Handle;
	};

	
}