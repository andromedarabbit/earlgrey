#pragma once
#include "Uncopyable.h"
#include "IPEndPoint.h"
#include "Socket.h"

namespace Earlgrey
{
	class TcpListener : private Uncopyable
	{
	public:
		explicit TcpListener(const IPEndPoint& localEP);
		~TcpListener();


		void Start();
		void Start(int backLog);

		void Stop();

		BOOL ExclusiveAddressUse() const;
		void ExclusiveAddressUse(BOOL use);

		BOOL Active() const;

		const IPEndPoint& LocalEndPoint() const;

	private:
		IPEndPoint m_ServerSocketEP;
		Socket * m_ServerSocket;
		BOOL m_Active;
		BOOL m_ExclusiveAddressUse;

	};

}
