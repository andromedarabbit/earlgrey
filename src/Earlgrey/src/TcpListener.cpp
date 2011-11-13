#include "stdafx.h"
#include "TcpListener.h"

namespace Earlgrey
{
	TcpListener::TcpListener(const IPEndPoint& localEP)
		: m_ServerSocketEP(localEP.Address(), localEP.Port())
		, m_ServerSocket( new Socket(m_ServerSocketEP.GetAddressFamily(), SocketType::Stream, ProtocolType::Tcp) )
		, m_Active(FALSE)
		, m_ExclusiveAddressUse(FALSE)

	{
		
	}

	TcpListener::~TcpListener()
	{
		delete m_ServerSocket;
	}

	void TcpListener::Start()
	{
		Start(SOMAXCONN);
	}

	void TcpListener::Start(int backLog)
	{
		if(backLog < 0 || backLog > SOMAXCONN)
			throw std::exception("invalid range of backlog value");


		if(m_Active == TRUE)
			throw std::exception("TcpListener already started!");

		EARLGREY_ASSERT(m_ServerSocket != NULL);
		m_ServerSocket->Bind(m_ServerSocketEP);
		m_ServerSocket->Listen(backLog);
		m_Active = TRUE;
		
	}

	void TcpListener::Stop()
	{
		m_Active = FALSE;

		delete m_ServerSocket;
		m_ServerSocket = new Socket(m_ServerSocketEP.GetAddressFamily(), SocketType::Stream, ProtocolType::Tcp);
		if(m_ExclusiveAddressUse)
		{
			m_ServerSocket->ExclusiveAddressUse(TRUE);
		}
	}

	BOOL TcpListener::ExclusiveAddressUse() const
	{
		return m_ExclusiveAddressUse;
	}

	void TcpListener::ExclusiveAddressUse(BOOL use)
	{
		m_ExclusiveAddressUse = use;
		m_ServerSocket->ExclusiveAddressUse(use);
	}

	BOOL TcpListener::Active() const
	{
		return m_Active;
	}

	const IPEndPoint& TcpListener::LocalEndPoint() const
	{
		return m_ServerSocketEP;
	}
}
