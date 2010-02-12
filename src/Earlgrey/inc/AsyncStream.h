#pragma once 
#include <WinSock2.h>

namespace Earlgrey
{
	class CompletionHandler;
	class ConnectionHandler;
	class NetworkBuffer;
	class Acceptor;
	class Connector;
	class Disconnector;
	class Sender;
	class Receiver;
	class Proactor;

	class AsyncOperation
	{
	public:
		//! operation �ʱ�ȭ
		/*!
			\param handler I/O�� �Ϸ�� �� ó���� �ڵ鷯(hook) ��ü
			\param handle I/O�� ó���� socket
			\param proactor I/O�� �Ϸ�� �� ������ handler�� ȣ�����ִ� demultiplexer
		*/
		void Initialize(CompletionHandler* handler, SOCKET handle, Proactor* proactor);

		//! post�� I/O ��û�� ����Ѵ�.
		void Cancel()
		{

		}

	protected:
		CompletionHandler* _handler;
		SOCKET _handle;
		Proactor* _proactor;
		NetworkBuffer* _buffer;
	};

	class AsyncReadStream : public AsyncOperation
	{
	public:
		bool Read();
	};

	class AsyncWriteStream : public AsyncOperation
	{
	public:
		bool Write();
	};

	class AsyncStream
	{
	public:
		explicit AsyncStream() ;		
		virtual ~AsyncStream() {}; //! \todo Close() �� �ҷ��� �ǳ�?

		BOOL Accept(USHORT Port); // Socket socket -> HANDLE handle ?
		BOOL Connect(const char* RemoteHostName, const INT Port);

		BOOL Recv(); 
		BOOL Send();
		BOOL Disconnect();
		BOOL Reuse();
		
		BOOL Post(CompletionHandler* Handler);

		NetworkBuffer* GetNetworkBuffer() { return _PacketBuffer; }
		SOCKET GetSocket() const {return _Handle; }

		virtual void Connected(){}//TODO : ���� �ѱ��
		virtual void Disconnected(){}
		virtual void Received(){}
		virtual void Sent(){}

	protected:
		inline SOCKET Socket() const
		{
			return _Handle;
		}
	private:
		NetworkBuffer* _PacketBuffer;
		SOCKET _Handle;

		Acceptor* _Acceptor;
		Connector* _Connector;
		Receiver* _Receiver;
		Sender* _Sender;
		Disconnector* _Disconnector;
	};
}