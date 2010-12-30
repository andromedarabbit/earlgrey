#pragma once 

#include "CompletionHandler.h"

namespace Earlgrey
{
	class NetworkBuffer;
	class Sender;
	class Receiver;
	class Proactor;
	class AsyncResult;
	class CompletionHandler;

	
	//! 비동기 I/O Stream
	/*!
		IOCP 입출력을 처리한다. 버퍼와 결과 정보가 읽기용과 쓰기용이 따로 분리돼 있다.
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

		bool Read();

		bool Write();

		//! 핸들(소켓)을 닫는다.
		void Close();

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