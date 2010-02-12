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
		//! operation 초기화
		/*!
			\param handler I/O가 완료된 후 처리될 핸들러(hook) 객체
			\param handle I/O를 처리할 socket
			\param proactor I/O가 완료된 후 적당한 handler를 호출해주는 demultiplexer
		*/
		void Initialize(CompletionHandler* handler, SOCKET handle, Proactor* proactor);

		//! post된 I/O 요청을 취소한다.
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
		virtual ~AsyncStream() {}; //! \todo Close() 안 불러도 되나?

		BOOL Accept(USHORT Port); // Socket socket -> HANDLE handle ?
		BOOL Connect(const char* RemoteHostName, const INT Port);

		BOOL Recv(); 
		BOOL Send();
		BOOL Disconnect();
		BOOL Reuse();
		
		BOOL Post(CompletionHandler* Handler);

		NetworkBuffer* GetNetworkBuffer() { return _PacketBuffer; }
		SOCKET GetSocket() const {return _Handle; }

		virtual void Connected(){}//TODO : 버퍼 넘기기
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