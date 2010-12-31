#pragma once

namespace Earlgrey {

	class NetworkBuffer;
	class AsyncStream;

	class IPacketHandler
	{
	public:
		//! 패킷을 처리한다.
		/*!
			버퍼에는 이미 처리한 데이터까지 포함돼 있다. 처리할 데이터는 Start 와 End 사이에 담겨 있으므로,
			이 범위를 벗어나서는 안된다. 패킷처리를 한 후에 처리한 데이터의 양을 출력 파라미터인 HandledSize 에 대입해야 한다.
			만약, 처리하기에 데이터의 양이 너무 적다면 HandleSize 에 0 을 대입해서 다음 패킷을 기다릴 수 있다.

			\param Stream AsyncStream 객체; 패킷을 전송하기 위해 필요하다.
			\param Buffer 패킷 데이터가 담겨있다.
			\param Start 처리할 수 있는 데이터가 담겨있는 버퍼의 시작 위치
			\param End 처리할 수 있는 데이터가 담겨있는 버퍼의 끝 위치
			\param HandledSize [out] 처리한 데이터의 크기

			\return 처리에 성공하면 true; false 를 리턴하면 연결이 끊긴다.
		*/
		virtual bool Handle(std::tr1::shared_ptr<AsyncStream> Stream, NetworkBuffer* Buffer, size_t Start, size_t End, size_t& HandledSize) = 0;
	};

}