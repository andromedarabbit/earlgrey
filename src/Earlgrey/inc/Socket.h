#pragma once 
#include "AddressFamily.h"
#include "SocketType.h"
#include "ProtocolType.h"

#include "ScopedLock.h"
#include <Loki/Threads.h>

#include <winsock2.h>


#ifdef EARLGREY_UNIT_TEST
#include <gtest/gtest_prod.h> // 단위 테스트
#endif 


namespace Earlgrey
{
	class IPAddress;
	class IPAddress2;
	class IPEndPoint;

	class Socket : private Uncopyable
	{
		// TODO 임시
		// friend class Socket;
		friend class IPAddress;

		friend class Dns;
		friend class IPAddress2;

#ifdef EARLGREY_UNIT_TEST
		// FRIEND_TEST(SocketTest, CreateTcpSocket);
		// FRIEND_TEST(SocketTest, Bind);
		
		friend class SocketTest;
#endif 

	public:
		explicit Socket(); // 편의상 제공하는 기본 생성자.
		explicit Socket(AddressFamily::E addressFamily, SocketType::E socketType, ProtocolType::E protocolType);
		~Socket();

		void Bind(const IPEndPoint& localEP);

		void Listen(int MaxConnections = SOMAXCONN);

		BOOL IsBound() const;
		

		// TODO: 핸들을 넘겨주는 메서드를 제공해도 되는 건가?
		SOCKET GetHandle() const;
		

		void ExclusiveAddressUse(BOOL use) const;
		BOOL ExclusiveAddressUse() const;

		void ReuseAddress(BOOL reuse) const;
		BOOL ReuseAddress() const;

	private:
		static void InitializeSockets();
		static void UninitializeSockets();

		// static SOCKET CreateWSASocket(AddressFamily::E addressFamily, SocketType::E socketType, ProtocolType::E protocolType);		

	private:
		typedef Loki::Mutex mutex_type;
		typedef ScopedLock<mutex_type> scoped_lock_type;
		static mutex_type s_InternalSyncObject;

		static BOOL s_Initialized;

		SOCKET m_Handle;
		AddressFamily::E m_AddressFamily;
		SocketType::E m_SocketType;
		ProtocolType::E m_ProtocolType;

		BOOL m_IsListening;

		IPEndPoint * m_EndPointBound;
	};

//	class Socket : private Uncopyable
//	{
//	public:
//		explicit Socket() 
//			: _Handle(INVALID_SOCKET)
//			, m_IsListening(false)
//		{
//			Socket2::InitializeSockets();
//		}
//
//		explicit Socket(SOCKET handle)
//			: _Handle(handle)
//			, m_IsListening(false)
//		{
//			Socket2::InitializeSockets();
//		}
//
//		void Close()
//		{
//			if (INVALID_SOCKET != _Handle)
//			{
//				closesocket( _Handle );
//				_Handle = INVALID_SOCKET;
//			}
//		}
//
//		bool Bind(USHORT Port)
//		{
//			SOCKADDR_IN Address;
//			Address.sin_family = AF_INET;
//			Address.sin_addr.s_addr = htonl(INADDR_ANY);
//			Address.sin_port = htons((u_short)Port);
//
//			if(bind(_Handle, (const struct sockaddr*) &Address, sizeof(Address)) != SOCKET_ERROR)
//				return true;
//
//			// TODO: 임시코드
//			const DWORD errCode = WSAGetLastError();
//			const TCHAR * const msg = Log::ErrorMessage(errCode);
//			DBG_UNREFERENCED_LOCAL_VARIABLE(msg);
//			return false;
//
//		}
//
//		// TODO: 뭔가 잘못 구현한 듯... 설계가...
//		bool Bind(const IPEndPoint& localEP);
//	
//
//		void BeginConnect(const IPEndPoint& remoteEP)
//		{
//			DBG_UNREFERENCED_PARAMETER(remoteEP);
//			if(m_IsListening)
//				throw std::exception("net_sockets_mustnotlisten");
//
//
//			if(CreateAsyncTcpSocket() == false)
//				throw std::exception("");
//
//			if(SetNonBlockingSocket() == false)
//				throw std::exception("");
//		}
//
//
//
//		bool Listen(int MaxConnections = SOMAXCONN)
//		{
//			if( listen(_Handle, MaxConnections) != SOCKET_ERROR )
//			{
//				m_IsListening = true;
//				return true;
//			}
//			return false;
//		}
//
//		bool SetReceiveBufferSize(INT Size) const
//		{
//			EARLGREY_ASSERT(Size >= 0);
//			return SOCKET_ERROR != setsockopt( _Handle, SOL_SOCKET, SO_RCVBUF, (const char*)&Size, sizeof(Size) );
//		}
//
//		bool SetSendBufferSize(INT Size) const
//		{
//			EARLGREY_ASSERT(Size >= 0);
//			return SOCKET_ERROR != setsockopt( _Handle, SOL_SOCKET, SO_SNDBUF, (const char*)&Size, sizeof(Size) );
//		}
//
//		SOCKET GetHandle() const
//		{
//			return _Handle;
//		}
//
//		bool SetReuseAddress() const
//		{
//			EARLGREY_ASSERT( _Handle != INVALID_SOCKET );
//			if (INVALID_SOCKET == _Handle)
//			{
//				return false;
//			}
//			BOOL Value = TRUE;
//			return SOCKET_ERROR != setsockopt(_Handle, SOL_SOCKET, SO_REUSEADDR, (const char*)&Value, sizeof(Value));
//		}
//
//		bool SetExclusiveAddressUse() const
//		{
//			EARLGREY_ASSERT( _Handle != INVALID_SOCKET );
//			if (INVALID_SOCKET == _Handle)
//			{
//				return false;
//			}
//			BOOL Value = TRUE;
//			return SOCKET_ERROR != setsockopt( _Handle, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (const char*)&Value, sizeof(Value));
//		}
//
//		bool SetNonBlockingSocket() const 
//		{
//			DWORD temp = 1;
//			return (ioctlsocket( _Handle, FIONBIO, &temp ) != SOCKET_ERROR);
//		}
//
//		bool IsValid() const
//		{
//			return _Handle != INVALID_SOCKET;
//		}
//
//	// private:
//	private:
//		friend class SocketHelper;
//
//#ifdef EARLGREY_UNIT_TEST
//		FRIEND_TEST(SocketTest, CreateTcpSocket);
//		FRIEND_TEST(SocketTest, Bind);
//#endif 
//
//		bool CreateTcpSocket();
//		bool CreateAsyncTcpSocket();
//
//
//	private:
//		// static BOOL s_UseOverlappedIO;
//		
//
//		SOCKET _Handle;
//		bool m_IsListening;
//	};

	
}