#include "stdafx.h"

#include "ServerInit.h"
#include "Acceptor.h"
#include "NetworkBuffer.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")


namespace Earlgrey
{
	void Acceptor::HandleEvent(AsyncResult* Result, DWORD TransferredBytes)
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

		CreateAcceptSocket(Result->Stream());

		return;
	}

	BOOL Acceptor::CreateListenSocket()
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

		if (!ProactorSingleton::Instance().RegisterHandler( (HANDLE)ListenSocket, this))
		{
			if (ListenSocket != INVALID_SOCKET) // this line is not needed
			{
				closesocket(ListenSocket);
			}
			return FALSE;
		}

		/*INT Zero = 0; 
		setsockopt(ListenSocket, 
		SOL_SOCKET, 
		SO_RCVBUF, 
		(const char*)&Zero, 
		sizeof(Zero));
		*/
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

	SOCKET Acceptor::CreateAcceptSocket(AsyncStream* InStream)
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
		if(WSAIoctl(ListenSocket, 
			SIO_GET_EXTENSION_FUNCTION_POINTER, 
			&GuidAcceptEx, 
			sizeof(GuidAcceptEx),
			&lpfnAcceptEx, 
			sizeof(lpfnAcceptEx), 
			&dwBytes, 
			NULL, 
			NULL) == SOCKET_ERROR)
			return INVALID_SOCKET;

		AcceptSocket =  WSASocket(AF_INET, 
			SOCK_STREAM, 
			0, 
			NULL, 
			0, 
			WSA_FLAG_OVERLAPPED);//socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (AcceptSocket == INVALID_SOCKET) {
			WSACleanup();
			return INVALID_SOCKET;
		}

		/*		INT Zero = 0; 
		setsockopt(AcceptSocket, 
		SOL_SOCKET, 
		SO_RCVBUF, 
		(const char*)&Zero, 
		sizeof(Zero));
		*/
		AsyncResult* Overlapped = new AsyncResult(this, InStream);// TODO : actor ��ü���� overlapped delete ����?
		//NetworkBuffer* Buf = Overlapped->Stream()->GetNetworkBuffer();
		TCHAR buf[1024];
		if( lpfnAcceptEx(ListenSocket, 
			AcceptSocket,
			buf,//Buf->GetAcceptBuffer(),
			1024-((sizeof(sockaddr_in) + 16) * 2),//Buf->GetAcceptBufferSize() - ((sizeof(sockaddr_in) + 16) * 2),
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

		if (!ProactorSingleton::Instance().RegisterHandler( (HANDLE)AcceptSocket, this))
		{
			if (AcceptSocket != INVALID_SOCKET) // this line is not needed
			{
				closesocket(AcceptSocket);
			}
			return FALSE;
		}
		return AcceptSocket;
	}


}