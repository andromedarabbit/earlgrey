#pragma once 
#include "Log.h"
#include "IPAddress.h"
#include "IPEndPoint.h"
#include "RAII.h"
#include "numeric_cast.hpp"

#include <Loki/Threads.h>

#include <winsock2.h>


#pragma warning( push ) 
#pragma warning( disable : 4996 )
#include <ws2tcpip.h>
#pragma warning( pop ) 

// #pragma comment(lib, "Ws2_32.lib")


namespace Earlgrey
{
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

		bool Bind(const IPEndPoint& localEP)
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
			const int retVal = ::GetAddrInfo(localEP.Address().ToString().c_str(), NULL, &aiHints, &aiList);		
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

				EARLGREY_ASSERT(IsValid());
				_Handle = handle;

				return true;
			} while ( NULL != (current = current->ai_next) );

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
		static mutex_type s_InternalSyncObject;

		SOCKET _Handle;
	};

	
}