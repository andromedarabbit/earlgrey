#pragma once

namespace Earlgrey {

	class Connection;

	class INetEvent
	{
	public:
		//! ������ �������� �� ȣ��ȴ�.
		virtual void OnConnected(std::tr1::shared_ptr<Connection> connection) = 0;

		//! ������ ������ �� ȣ��ȴ�.
		virtual void OnDisconnected() = 0;
	};

}