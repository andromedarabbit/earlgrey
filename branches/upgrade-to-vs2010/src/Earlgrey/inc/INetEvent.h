#pragma once

namespace Earlgrey {

	class Connection;
	class AsyncStream;

	class INetEvent
	{
	public:
		//! ������ �������� �� ȣ��ȴ�.
		virtual void OnConnected(std::tr1::shared_ptr<Connection> connection) = 0;

		//! ������ ������ �� ȣ��ȴ�.
		virtual void OnDisconnected(__int64 tag) = 0;
	};

}