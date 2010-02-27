#pragma once 
#include <WinSock2.h>

namespace Earlgrey
{
	class CompletionHandler;
	class NetworkBuffer;
	class Sender;
	class Receiver;
	class Proactor;
	class AsyncResult;

	
	//! �񵿱� I/O Stream
	/*!
		IOCP ������� ó���Ѵ�.
	*/
	class AsyncStream : public CompletionHandler
	{
	public:
		//! operation �ʱ�ȭ
		/*!
			\param handle I/O�� ó���� socket
			\param readHandler Receive�� �Ϸ�� �� ó���� �ڵ鷯(hook) ��ü
			\param writeHandler Send�� �Ϸ�� �� ó���� �ڵ鷯(hook) ��ü
			\param proactor I/O�� �Ϸ�� �� ������ handler�� ȣ�����ִ� demultiplexer
		*/
		void Initialize(SOCKET handle, CompletionHandler* readHandler, CompletionHandler* writeHandler, Proactor* proactor);

		//! post�� I/O ��û�� ����Ѵ�.
		void Cancel()
		{
		}

		bool Read();

		bool Write();

		//! �ڵ�(����)�� �ݴ´�.
		void Close()
		{
			closesocket( _handle );
		}

		virtual void HandleEvent(AsyncResult* Result);

		NetworkBuffer* GetReadBuffer();

		NetworkBuffer* GetWriteBuffer();

	protected:
		SOCKET _handle;
		Proactor* _proactor;
		std::auto_ptr<NetworkBuffer> _bufferForRead;
		std::auto_ptr<NetworkBuffer> _bufferForWrite;
		std::auto_ptr<AsyncResult> _resultForRead;
		std::auto_ptr<AsyncResult> _resultForWrite;
	};

	
	//class AsyncStream
	//{
	//public:
	//	explicit AsyncStream();

	//	virtual ~AsyncStream() 
	//	{
	//	}

	//	BOOL Accept(USHORT Port); // Socket socket -> HANDLE handle ?
	//	BOOL Connect(const char* RemoteHostName, const INT Port);

	//	BOOL Recv(); 
	//	BOOL Send();
	//	BOOL Disconnect();
	//	BOOL Reuse();
	//	
	//	BOOL Post(CompletionHandler* Handler);

	//	NetworkBuffer* GetNetworkBuffer() { return _PacketBuffer; }
	//	SOCKET GetSocket() const {return _Handle; }

	//	virtual void Connected(){}//TODO : ���� �ѱ��
	//	virtual void Disconnected(){}
	//	virtual void Received(){}
	//	virtual void Sent(){}

	//protected:
	//	inline SOCKET Socket() const
	//	{
	//		return _Handle;
	//	}
	//private:
	//	NetworkBuffer* _PacketBuffer;
	//	SOCKET _Handle;

	//	Acceptor* _Acceptor;
	//	Connector* _Connector;
	//	Receiver* _Receiver;
	//	Sender* _Sender;
	//	Disconnector* _Disconnector;
	//};
}