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

	
	//! 비동기 I/O Stream
	/*!
		IOCP 입출력을 처리한다.
	*/
	class AsyncStream : public CompletionHandler
	{
	public:
		//! operation 초기화
		/*!
			\param handle I/O를 처리할 socket
			\param readHandler Receive가 완료된 후 처리될 핸들러(hook) 객체
			\param writeHandler Send가 완료된 후 처리될 핸들러(hook) 객체
			\param proactor I/O가 완료된 후 적당한 handler를 호출해주는 demultiplexer
		*/
		void Initialize(SOCKET handle, CompletionHandler* readHandler, CompletionHandler* writeHandler, Proactor* proactor);

		//! post된 I/O 요청을 취소한다.
		void Cancel()
		{
		}

		bool Read();

		bool Write();

		//! 핸들(소켓)을 닫는다.
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

	//	virtual void Connected(){}//TODO : 버퍼 넘기기
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