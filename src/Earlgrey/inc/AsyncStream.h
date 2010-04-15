#pragma once 
#include "Proactor.h" // CompletionHandler
#include <WinSock2.h>

namespace Earlgrey
{
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

}