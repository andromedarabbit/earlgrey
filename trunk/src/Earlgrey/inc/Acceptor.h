#pragma once 
#include "EarlgreyAssert.h"
#include "AsyncStream.h"
#include "mswsock.h"
#include "NetworkBuffer.h"
#include "ThreadRunnable.h"
#include "WaitEvent.h"


namespace Earlgrey
{

	//! Accept 이벤트를 대기하는 스레드의 runnable 클래스
	/*!
		Listen 또는 Connect를 여러 개 하더라도 Accept 이벤트는 이 스레드에서만 기다린다.
		
	*/
	/*class AcceptorRunnable : public ThreadRunnable
	{
	public:
		AcceptorRunnable() : _IsRunning(0L)
		{
		}

		BOOL Init()
		{
			_IsRunning = 1L;
			return __super::Init();
		}

		virtual void Stop() 
		{
			::InterlockedExchange( &_IsRunning, 0L );
		}

		virtual void Exit() 
		{
		}

		BOOL MeetsStopCondition() const
		{
			return !_IsRunning;
		}

		DWORD DoTask() 
		{
			WaitEventContainerSingleton::Instance().WaitEvents();
			return 0;
		}

	private:
		volatile LONG _IsRunning;
	};*/


}