#pragma once 

#include "CompletionHandler.h"

namespace Earlgrey
{
	class AsyncStream;
	class AsyncResult;

	class Receiver : public CompletionHandler
	{
	public:
		explicit Receiver(AsyncStream* Stream);

		virtual ~Receiver();

		// CompletionHandler Interface
		virtual void HandleEvent(AsyncResult* Result);

		virtual void OnDisconnected();

		//! 패킷 핸들러; 패킷 파싱 등을 구현해야 한다. 
		/*!
			패킷을 처리했다면 _Start 멤버를 적당한 위치로 이동시켜야 한다.
		*/
		virtual bool HandlePacket(NetworkBuffer* buffer);
	protected:
		AsyncStream* _Stream;
		size_t _Start;	//!< 버퍼에서 처리할 데이터의 시작 위치
		size_t _End;	//!< 버퍼에 기록된 데이터의 끝을 가리킨다.
	};
}