#pragma once 
#include <winsock2.h>

namespace Earlgrey
{
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

	class Socket
	{
	public:
		Socket() : _Handle(INVALID_SOCKET)
		{
		}

		SOCKET Attach(SOCKET Handle)
		{
			std::swap( Handle, _Handle );
			return Handle;
		}

		bool CreateTcpSocket()
		{
			_Handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			return _Handle != INVALID_SOCKET;
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

			return bind(_Handle, (const struct sockaddr*) &Address, sizeof(Address)) != SOCKET_ERROR;
		}

		bool Listen(int MaxConnections = SOMAXCONN)
		{
			return listen(_Handle, MaxConnections) != SOCKET_ERROR;
		}

		bool SetReceiveBufferSize(INT Size) const
		{
			return SOCKET_ERROR != setsockopt( _Handle, SOL_SOCKET, SO_RCVBUF, (const char*)&Size, sizeof(Size) );
		}

		bool SetSendBufferSize(INT Size) const
		{
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
			return (ioctlsocket( _Handle, FIONBIO, &temp ) == SOCKET_ERROR);
		}

		bool IsValid() const
		{
			return _Handle != INVALID_SOCKET;
		}
	private:
		SOCKET _Handle;
	};

	
}