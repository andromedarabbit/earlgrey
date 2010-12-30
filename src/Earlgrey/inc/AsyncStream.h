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

	
	//! �񵿱� I/O Stream
	/*!
		IOCP ������� ó���Ѵ�. ���ۿ� ��� ������ �б��� ������� ���� �и��� �ִ�.
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

		bool Read();

		bool Write();

		//! �ڵ�(����)�� �ݴ´�.
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