#pragma once 
#include "Log.h"
#include "IPAddress.h"
#include "IPEndPoint.h"
#include "RAII.h"
#include "numeric_cast.hpp"
#include "ScopedLock.h"

#include <Loki/Threads.h>

#include <winsock2.h>


#pragma warning( push ) 
#pragma warning( disable : 4996 )
#include <ws2tcpip.h>
#pragma warning( pop ) 

// #pragma comment(lib, "Ws2_32.lib")

#ifdef EARLGREY_UNIT_TEST
#include <gtest/gtest_prod.h> // 단위 테스트
#endif 

namespace Earlgrey
{
	class Socket : private Uncopyable
	{
	public:
		typedef Loki::Mutex mutex_type;
		typedef ScopedLock<mutex_type> scoped_lock_type;

		explicit Socket() 
			: _Handle(INVALID_SOCKET)
			, m_IsListening(false)
		{
			Socket::InitializeSockets();
		}

		explicit Socket(SOCKET handle)
			: _Handle(handle)
			, m_IsListening(false)
		{
			Socket::InitializeSockets();
		}

		void Close()
		{
			if (INVALID_SOCKET != _Handle)
			{
				closesocket( _Handle );
				_Handle = INVALID_SOCKET;
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
			const TCHAR * const msg = Log::ErrorMessage(errCode);
			DBG_UNREFERENCED_LOCAL_VARIABLE(msg);
			return false;

		}

		// TODO: 뭔가 잘못 구현한 듯... 설계가...
		bool Bind(const IPEndPoint& localEP);
	

		void BeginConnect(const IPEndPoint& remoteEP)
		{
			DBG_UNREFERENCED_PARAMETER(remoteEP);
			if(m_IsListening)
				throw std::exception("net_sockets_mustnotlisten");


			if(CreateAsyncTcpSocket() == false)
				throw std::exception("");

			if(SetNonBlockingSocket() == false)
				throw std::exception("");
		}



		bool Listen(int MaxConnections = SOMAXCONN)
		{
			if( listen(_Handle, MaxConnections) != SOCKET_ERROR )
			{
				m_IsListening = true;
				return true;
			}
			return false;
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
		friend class SocketHelper;

#ifdef EARLGREY_UNIT_TEST
		FRIEND_TEST(SocketTest, CreateTcpSocket);
		FRIEND_TEST(SocketTest, Bind);
#endif 

		bool CreateTcpSocket();
		bool CreateAsyncTcpSocket();


	private:
		static BOOL s_Initialized;
		// static BOOL s_UseOverlappedIO;
		static mutex_type s_InternalSyncObject;

		SOCKET _Handle;
		bool m_IsListening;
	};

	
}