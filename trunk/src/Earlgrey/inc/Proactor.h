#pragma once 

#include <Loki/Singleton.h>
#include <Loki/Threads.h> // for Loki::SingleThreaded
#include <vector>
#include <map>

#include "NoLock.h"
#include "TimeSpan.h"
#include "xlist.h"

namespace Earlgrey
{
	class NetworkBuffer;
	class AsyncResult;
	class CompletionHandler;
	class AppSettings;
	class CompletionHandler;

	//Windows IOCompletion Port ��
	class Proactor : private Uncopyable
	{
	public:
		explicit Proactor() {}
		virtual ~Proactor() {}

		// virtual BOOL HandleEvent(TimeValueType WaitTime) = 0;
		virtual BOOL HandleEvent(TimeSpan WaitTime);
		virtual BOOL HandleEvent(DWORD milliseconds) = 0;

		//! TODO: �̷��� �ü�� �ڵ��� �巯������ �ְ� �޾Ƶ� �ǳ�?  
		virtual BOOL RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler) = 0;
		virtual BOOL DeregisterHandler(HANDLE Handle) = 0;
	};

	class WinProactor 
		: public Proactor
	{
	public:
		explicit WinProactor();		
		virtual ~WinProactor();

		BOOL Initialize(const AppSettings& appSettings);
		
		// Proactor Pattern Interface
		virtual BOOL HandleEvent(TimeSpan WaitTime = TimeSpan::MaxValue);
		virtual BOOL HandleEvent(DWORD WaitTime = INFINITE); 
		virtual BOOL RegisterHandler(HANDLE Handle, CompletionHandler* CompleteHandler);
		virtual BOOL DeregisterHandler(HANDLE Handle);

		BOOL PostEvent(AsyncResult* Result);

	private:
		HANDLE _IOCompletionPort;
	};

	typedef
		Loki::SingletonHolder<WinProactor, Loki::CreateUsingNew, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
			ProactorSingleton
			;

	
	//! Async I/O ó���� ���� �Ѱ����� token
	/*!
		Read / Write ���� �������� ����ϴ� token�̴�.
		�Ϸ�Ǹ� �� token�� ������ ������ ä���� �� �ڵ鷯���� �Ѱ�����.
	*/
	class AsyncResult
	{
		friend class WinProactor;
		friend class AsyncStream;

		typedef std::tr1::shared_ptr<NetworkBuffer>		BufferPtr;
	public:
		// new/delete ������ �����ε� default allocator �� ����ϵ��� �Ѵ�.
		static void* operator new(size_t size);
		static void operator delete(void* p);
	private:
		// �迭 �����ڴ� ���´�.
		static void* operator new[](size_t);
		static void operator delete[](void*);
	public:
		AsyncResult();

		//! I/O �� ��� ���� ��ü�� ť�� ���� �� ����ϴ� ������
		explicit AsyncResult(CompletionHandler* Handler);

		//! I/O ó���� ����ϴ� ������
		AsyncResult(SOCKET Handle, CompletionHandler* Handler);

		//! Send Result ���� ����ϴ� ������
		AsyncResult(SOCKET Handle, CompletionHandler* Handler, BufferPtr Buffer);

		~AsyncResult();

		DWORD GetBytesTransferred() const
		{
			return _BytesTransferred;
		}

		SOCKET GetHandle() const
		{
			return _Handle;
		}

		//! �ڵ鷯�� ȣ���Ѵ�. Receiver / Sender ..
		void HandleEvent();

		CompletionHandler* GetHandler()
		{
			return _Handler;
		}

		OVERLAPPED* GetOverlapped()
		{
			return &_overlapped;
		}

		DWORD GetError() const
		{
			return _Error;
		}

		bool SentCompleted();

	private:
		void SetBytesTransferred(DWORD Bytes)
		{
			_BytesTransferred = Bytes;
		}

		void SetErrorCode(DWORD Error)
		{
			_Error = Error;
		}

		//! ����� ���ۿ� ���۵� ũ�⸦ ������Ʈ�Ѵ�.
		void UpdateSentSize();
		
		//! WSASend ���� ����ϴ� ����� ���۸� ��´�.
		/*!
			�����ϰ� ���� �����Ϳ� ���� ���۸��� ��´�. UpdateSentSize() �� ȣ���ϸ� ������ ���۵� �������� ���� �����Ѵ�.
			���� ���۵� �������� ��� ������ �������� ���� ������ ��� �����Ͱ� ���۵� �������� �׷��� �ʴٸ� ���� �����Ͱ�
			���Ҵٴ� ���� �ǹǷ� ������ �õ��ؾ� �Ѵ�.

			\sa UpdateSentSize
		*/
		std::pair<WSABUF*,DWORD> GetWriteBuffer();

	private:
		OVERLAPPED _overlapped;
		CompletionHandler* _Handler;
		DWORD _BytesTransferred;
		SOCKET _Handle;
		DWORD _Error;
		BufferPtr _Buffer;	//!< ������ �����Ͱ� ����ִ� ����; ��� �������� ���� ���� �ֱ� ������ �ʿ��ϴ�.
	};

}