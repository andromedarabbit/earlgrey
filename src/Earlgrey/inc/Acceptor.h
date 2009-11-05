#pragma once 
#include "EarlgreyAssert.h"
#include "AsyncStream.h"
// #include "IOCPEventType.h"
#include "Proactor.h"
#include "mswsock.h"
#include "NetworkBuffer.h"

namespace Earlgrey
{
	class Acceptor 
		: public CompletionHandler
	{
	public:
		explicit Acceptor(USHORT InPort)
			: ListenSocket(INVALID_SOCKET),
			Port(InPort)
		{};

		virtual ~Acceptor() {};

		BOOL Register();		

		// CompletionHandler Interface
		virtual void HandleEvent(AsyncResult* Result, DWORD TransferredBytes);
		virtual void HandleEventError(AsyncResult* Result, DWORD Error);

		BOOL CreateListenSocket();
		SOCKET CreateAcceptSocket();

	private:
		SOCKET ListenSocket;		
		SOCKET AcceptSocket;
		USHORT Port;
	};


	/**
	*/
	inline BOOL Acceptor::Register()
	{
		if (!ProactorSingleton::Instance().RegisterHandler( (HANDLE)AcceptSocket, this))
		{
			if (AcceptSocket != INVALID_SOCKET) // this line is not needed
			{
				closesocket(ListenSocket);//AcceptSocket��?
			}
			return FALSE;
		}
		return TRUE;
	}


	inline void Acceptor::HandleEvent(AsyncResult* Result, DWORD TransferredBytes)
	{
		/* Accept�� ������ Context�� Update��Ų��. (�����Ͽ��� Ȱ�� �������� ��ȯ) */
		if ((Result->Error() == (DWORD)-1) ||
			(setsockopt( AcceptSocket, 
			SOL_SOCKET, 
			SO_UPDATE_ACCEPT_CONTEXT, 
			(char *)&ListenSocket,
			sizeof(ListenSocket)) < 0))
		{
			return;
		}
		else 
		{
			if(TransferredBytes == 0)
			{
				closesocket(AcceptSocket);
				return;
			}

			SOCKADDR *LAddr = NULL, *RAddr = NULL;
			int       LLen = 0, RLen = 0;
			NetworkBuffer* Buffer = Result->Stream()->GetNetworkBuffer();
			/* ���ӵ� Ŭ���̾�Ʈ ������ �޴´�. */
			GetAcceptExSockaddrs( Buffer->GetAcceptBuffer(), Buffer->GetBufferCapacity(), sizeof(SOCKADDR_STORAGE) + 16, 
				sizeof(SOCKADDR_STORAGE) + 16, &LAddr, &LLen, &RAddr, &RLen); 
			{ 
				//TODO : ���̵� �ʿ���~ ����ڰ� ������ connectionŬ������ ����������ϴµ�... �̰� ���ø����� ���ڴ� �˴� Ŭ������ �� �ٲ�����ϳ�. �� �׷����ϳ�? --; �� ����ϰ� �ذ�� ��� ������?
				Result->Stream()->Connected();

				Result->Stream()->Recv();
			} 
		} 
		
		CreateAcceptSocket();

		return;
	}

	inline void Acceptor::HandleEventError(AsyncResult* /*Result*/, DWORD /*Error*/)
	{
		return;//TODO :
	}

	inline BOOL Acceptor::CreateListenSocket()
	{
		if( ListenSocket != INVALID_SOCKET )
		{
			return FALSE;
		}

		ListenSocket = WSASocket(AF_INET, 
			SOCK_STREAM, 
			0, 
			NULL, 
			0, 
			WSA_FLAG_OVERLAPPED);

		if (ListenSocket == INVALID_SOCKET)
		{
			// TODO: WSAGetLastError.
			return FALSE;
		}

		INT Zero = 0; 
		setsockopt(ListenSocket, 
			SOL_SOCKET, 
			SO_RCVBUF, 
			(const char*)&Zero, 
			sizeof(Zero));

		BOOL OptionValue = TRUE;
		setsockopt(ListenSocket, 
			SOL_SOCKET, 
			SO_REUSEADDR, 
			(const char*)&OptionValue, 
			sizeof(OptionValue));


		SOCKADDR_IN Address;
		Address.sin_family = AF_INET;
		Address.sin_addr.s_addr = htonl(INADDR_ANY);
		Address.sin_port = htons((u_short)Port);

		if (bind(ListenSocket, (const struct sockaddr*) &Address, sizeof(Address)) == SOCKET_ERROR)
		{
			// TODO: error
			// DWORD a = GetLastError();
			// UNREFERENCED_PARAMETER(a);
			return FALSE;
		}

		if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			return FALSE;
		}

		if (ListenSocket == INVALID_SOCKET)
		{
			//closesocket(ListenSocket);
			return FALSE;
		}

		return TRUE;
	}

	inline SOCKET Acceptor::CreateAcceptSocket()
	{

		// AcceptEx�� �� ���� ������ ���� ��ȯ���� ����.
		/* 
		accept �� �����ó���� ������ ������ 
		accept �� ���� wait thread �� iocp �� �и��Ǿ� �۾��Ϸ��� �̷� ������ ����߽��ϴ�. 
		�׷��� AcceptEX �� ������� ����.. 64���� �� ������ �� �� �ִ°� ����ϰ� ���ӵǰ� ����Ǵ� ��� connection timeout�� ���� �߻�.. 
		backlog �������ְ� wait thread �� accept�� �ϹǷ� ũ�� ������ �ȵ�����..
		*/


		LPFN_ACCEPTEX lpfnAcceptEx = NULL;
		GUID GuidAcceptEx = WSAID_ACCEPTEX;
		DWORD dwBytes;

		//----------------------------------------
		// Load the AcceptEx function into memory using WSAIoctl.
		// The WSAIoctl function is an extension of the ioctlsocket()
		// function that can use overlapped I/O. The function's 3rd
		// through 6th parameters are input and output buffers where
		// we pass the pointer to our AcceptEx function. This is used
		// so that we can call the AcceptEx function directly, rather
		// than refer to the Mswsock.lib library.
		WSAIoctl(ListenSocket, 
			SIO_GET_EXTENSION_FUNCTION_POINTER, 
			&GuidAcceptEx, 
			sizeof(GuidAcceptEx),
			&lpfnAcceptEx, 
			sizeof(lpfnAcceptEx), 
			&dwBytes, 
			NULL, 
			NULL);

		SOCKET AcceptSocket =  WSASocket(AF_INET, 
			SOCK_STREAM, 
			0, 
			NULL, 
			0, 
			WSA_FLAG_OVERLAPPED);//socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (AcceptSocket == INVALID_SOCKET) {
			WSACleanup();
			return INVALID_SOCKET;
		}

		INT Zero = 0; 
		setsockopt(AcceptSocket, 
			SOL_SOCKET, 
			SO_RCVBUF, 
			(const char*)&Zero, 
			sizeof(Zero));

		AsyncResult* Overlapped = new AsyncResult(this);// TODO : actor ��ü���� overlapped delete ����?
		NetworkBuffer* Buf = Overlapped->Stream()->GetNetworkBuffer();
		
		if( lpfnAcceptEx(ListenSocket, 
			AcceptSocket,
			Buf->GetAcceptBuffer(),
			Buf->GetBufferCapacity() - ((sizeof(sockaddr_in) + 16) * 2),
			sizeof(sockaddr_in) + 16, 
			sizeof(sockaddr_in) + 16, 
			&dwBytes, 
			Overlapped) == FALSE)
		{
			switch (WSAGetLastError())
			{
			case ERROR_IO_PENDING: 
				break;
			default : 
				closesocket(ListenSocket);//�ؾ��ϳ�?
				return INVALID_SOCKET;
			}
		}
		return AcceptSocket;
	}

	/*class ConnectionHandler 
	{
	public:
		explicit ConnectionHandler(HANDLE InHandle)
		{
			socket_ = (SOCKET)InHandle;
			Initialize();
		};

		BOOL Initialize();

		virtual ~ConnectionHandler() {};

		// from acceptor, connector
		virtual void Connected() {}
		// from disconnector
		virtual void Disconnected() {}
		// from sender
		virtual void Sent() {}
		//from receiver
		virtual void Received() {}

	protected:
		inline SOCKET Socket() const
		{
			return socket_;
		}

	private:
		SOCKET socket_;
	};
*/
}