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

	
	//! �񵿱� I/O Stream
	/*!
		IOCP ������� ó���Ѵ�. ���ۿ� ��� ������ �б��� ������� ���� �и��� �ִ�.
	*/
	class AsyncStream : public CompletionHandler, public Algorithm::Lockfree::TaskQueue<void()>
	{
		typedef xlist<AsyncResult*>::Type ResultListType;
	public:
		explicit AsyncStream();

		//! operation �ʱ�ȭ
		/*!
			\param handle I/O�� ó���� socket
			\param readHandler Receive�� �Ϸ�� �� ó���� �ڵ鷯(hook) ��ü
			\param writeHandler Send�� �Ϸ�� �� ó���� �ڵ鷯(hook) ��ü
			\param proactor I/O�� �Ϸ�� �� ������ handler�� ȣ�����ִ� demultiplexer
		*/
		void Initialize(SOCKET handle, CompletionHandler* readHandler, CompletionHandler* writeHandler, Proactor* proactor);

		bool Read();

		//! ���ۿ� �ִ� ������ ������ ��û�Ѵ�.
		void Write(std::tr1::shared_ptr<NetworkBuffer> buffer);

		//! �ڵ�(����)�� �ݴ´�.
		void Close();

		virtual void HandleEvent(AsyncResult* Result);

		NetworkBuffer* GetReadBuffer();


		//! ���� �Ϸ� �̺�Ʈ�� ���� ȣ��ȴ�.
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
		//! ���ۿ� �ִ� �����͸� �����Ѵ�.
		void _Write(std::tr1::shared_ptr<NetworkBuffer> buffer);

		//! ���ۿ� ���� �����͸� ������.
		/*!
			Sender �� ���� �Ϸ� �̺�Ʈ�� �޾� ���۵��� ���� �����Ͱ� ���� ���� ��� �� �����͸� �ٽ� ������.
		*/
		void _WriteRemainder();

		bool _Send(AsyncResult* result);
		
		void _OnSent(AsyncResult* Result);

	private:
		SOCKET _handle;
		Proactor* _proactor;

		//! �б⸦ ���� ���ۿ� ��� ��ü�� ���� ���ۿʹ� �޸� �ϳ��� ������ �ȴ�.
		std::auto_ptr<NetworkBuffer> _bufferForRead;
		std::auto_ptr<AsyncResult> _resultForRead;

		//! �� �����ʹ� task queue �� ��ȣ�ž� �Ѵ�.
		CompletionHandler* _writeHandler;
		ResultListType _ResultList;
		bool _Sending;

		__int64 _Tag; //!< ����ڰ� ���Ƿ� ����ϴ� ��; Connection ID ���� ��� �� �� �ִ�.

	};

}