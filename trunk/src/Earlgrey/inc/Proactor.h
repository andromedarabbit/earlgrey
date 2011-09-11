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

	//Windows IOCompletion Port 용
	class Proactor : private Uncopyable
	{
	public:
		explicit Proactor() {}
		virtual ~Proactor() {}

		// virtual BOOL HandleEvent(TimeValueType WaitTime) = 0;
		virtual BOOL HandleEvent(TimeSpan WaitTime);
		virtual BOOL HandleEvent(DWORD milliseconds) = 0;

		//! TODO: 이렇게 운영체제 핸들을 드러내놓고 주고 받아도 되나?  
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

	
	//! Async I/O 처리를 위해 넘겨지는 token
	/*!
		Read / Write 에서 공통으로 사용하는 token이다.
		완료되면 이 token이 적당한 값으로 채워진 후 핸들러에게 넘겨진다.
	*/
	class AsyncResult
	{
		friend class WinProactor;
		friend class AsyncStream;

		typedef std::tr1::shared_ptr<NetworkBuffer>		BufferPtr;
	public:
		// new/delete 연산자 오버로딩 default allocator 를 사용하도록 한다.
		static void* operator new(size_t size);
		static void operator delete(void* p);
	private:
		// 배열 연산자는 막는다.
		static void* operator new[](size_t);
		static void operator delete[](void*);
	public:
		AsyncResult();

		//! I/O 와 상관 없는 객체를 큐에 넣을 때 사용하는 생성자
		explicit AsyncResult(CompletionHandler* Handler);

		//! I/O 처리에 사용하는 생성자
		AsyncResult(SOCKET Handle, CompletionHandler* Handler);

		//! Send Result 에만 사용하는 생성자
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

		//! 핸들러를 호출한다. Receiver / Sender ..
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

		//! 쓰기용 버퍼에 전송된 크기를 업데이트한다.
		void UpdateSentSize();
		
		//! WSASend 에서 사용하는 쓰기용 버퍼를 얻는다.
		/*!
			전송하고 남은 데이터에 대한 버퍼만을 얻는다. UpdateSentSize() 를 호출하면 버퍼의 전송된 데이터의 양을 갱신한다.
			만약 전송된 데이터의 양과 전송할 데이터의 양이 같으면 모든 데이터가 전송된 것이지만 그렇지 않다면 보낼 데이터가
			남았다는 뜻이 되므로 전송을 시도해야 한다.

			\sa UpdateSentSize
		*/
		std::pair<WSABUF*,DWORD> GetWriteBuffer();

	private:
		OVERLAPPED _overlapped;
		CompletionHandler* _Handler;
		DWORD _BytesTransferred;
		SOCKET _Handle;
		DWORD _Error;
		BufferPtr _Buffer;	//!< 전송할 데이터가 담겨있는 버퍼; 모두 전송하지 못할 수도 있기 때문에 필요하다.
	};

}