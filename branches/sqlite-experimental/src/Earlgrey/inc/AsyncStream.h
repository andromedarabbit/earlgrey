#pragma once 

#include "CompletionHandler.h"
#include "taskqueue.h"

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
	class AsyncStream : public CompletionHandler, public Algorithm::Lockfree::TaskQueue<void()>
	{
		typedef xlist<AsyncResult*>::Type ResultListType;
	public:
		explicit AsyncStream();

		//! operation 초기화
		/*!
			\param handle I/O를 처리할 socket
			\param readHandler Receive가 완료된 후 처리될 핸들러(hook) 객체
			\param writeHandler Send가 완료된 후 처리될 핸들러(hook) 객체
			\param proactor I/O가 완료된 후 적당한 handler를 호출해주는 demultiplexer
		*/
		void Initialize(SOCKET handle, CompletionHandler* readHandler, CompletionHandler* writeHandler, Proactor* proactor);

		bool Read();

		//! 버퍼에 있는 데이터 전송을 요청한다.
		void Write(std::tr1::shared_ptr<NetworkBuffer> buffer);

		//! 핸들(소켓)을 닫는다.
		void Close();

		virtual void HandleEvent(AsyncResult* Result);

		NetworkBuffer* GetReadBuffer();


		//! 전송 완료 이벤트가 오면 호출된다.
		void OnSent(AsyncResult* Result);

		void SetTag(__int64 Tag)
		{
			_Tag = Tag;
		}

		__int64 GetTag() const
		{
			return _Tag;
		}
	private:
		//! 버퍼에 있는 데이터를 전송한다.
		void _Write(std::tr1::shared_ptr<NetworkBuffer> buffer);

		//! 버퍼에 남은 데이터를 보낸다.
		/*!
			Sender 는 전송 완료 이벤트를 받아 전송되지 않은 데이터가 남아 있을 경우 그 데이터를 다시 보낸다.
		*/
		void _WriteRemainder();

		bool _Send(AsyncResult* result);
		
		void _OnSent(AsyncResult* Result);

	private:
		SOCKET _handle;
		Proactor* _proactor;

		//! 읽기를 위한 버퍼와 결과 객체는 쓰기 버퍼와는 달리 하나만 있으면 된다.
		std::auto_ptr<NetworkBuffer> _bufferForRead;
		std::auto_ptr<AsyncResult> _resultForRead;

		//! 이 데이터는 task queue 로 보호돼야 한다.
		CompletionHandler* _writeHandler;
		ResultListType _ResultList;
		bool _Sending;

		__int64 _Tag; //!< 사용자가 임의로 사용하는 값; Connection ID 등을 담아 둘 수 있다.

	};

}