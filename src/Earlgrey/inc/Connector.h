#pragma once 
#include "EarlgreyAssert.h"
#include "Socket.h"
#include "AsyncStream.h"
#include "WaitEvent.h"
#include "IPAddress.h"

namespace Earlgrey
{
	class IPAddress;

	class Connector : public IWaitHandler
	{
	public:
		explicit Connector() : 
			_RetryCount(0), 
			_RetryInterval(0), 
			_Socket(INVALID_SOCKET),
			_WaitEvent(INVALID_HANDLE_VALUE),
			_Port(0)
		{
		}

		virtual ~Connector() 
		{
		}


		//! ���ῡ ������ �Ŀ� �翬���� �� �� �׸��� �� �ֱ�� ������ ���Ѵ�.
		/*!
			\param Count �翬�� �õ� ��
			\param Interval �õ� �ֱ�
		*/
		void SetRetry(BYTE Count, DWORD Interval)
		{
			_RetryCount = Count;
			_RetryInterval = Interval;
		}

		bool Connect(LPCSTR Server, INT Port);

		bool ReConnect();

		//! ���� �̺�Ʈ�� ���� ����Ǵ� �޼����̴�.
		/*!
			������ �����ϸ� ��õ��Ѵ�. �����ϸ� Read �۾��� �����Ѵ�.
		*/
		bool DoTask();

	private:
		void Close();

	private:
		Socket _Socket;
		std::string _ServerName;
		INT _Port;
		HANDLE _WaitEvent;
		IPAddress _ServerAddress;
		BYTE _RetryCount;
		DWORD _RetryInterval;
	};

}