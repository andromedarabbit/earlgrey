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


		//! 연결에 실패한 후에 재연결을 몇 번 그리고 얼마 주기로 할지를 정한다.
		/*!
			\param Count 재연결 시도 수
			\param Interval 시도 주기
		*/
		void SetRetry(BYTE Count, DWORD Interval)
		{
			_RetryCount = Count;
			_RetryInterval = Interval;
		}

		bool Connect(LPCSTR Server, INT Port);

		bool ReConnect();

		//! 연결 이벤트가 오면 수행되는 메서드이다.
		/*!
			연결이 실패하면 재시도한다. 성공하면 Read 작업을 시작한다.
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