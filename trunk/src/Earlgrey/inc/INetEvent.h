#pragma once

namespace Earlgrey {

	class Connection;

	class INetEvent
	{
	public:
		//! 연결이 성공했을 때 호출된다.
		virtual void OnConnected(std::tr1::shared_ptr<Connection> connection) = 0;

		//! 연결이 끊겼을 때 호출된다.
		virtual void OnDisconnected() = 0;
	};

}