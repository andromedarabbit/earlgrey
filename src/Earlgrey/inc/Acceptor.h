#pragma once 
#include "EarlgreyAssert.h"
#include "AsyncStream.h"
#include "mswsock.h"
#include "NetworkBuffer.h"
#include "ThreadRunnable.h"
#include "WaitEvent.h"


namespace Earlgrey
{

	//! Accept �̺�Ʈ�� ����ϴ� �������� runnable Ŭ����
	/*!
		Listen �Ǵ� Connect�� ���� �� �ϴ��� Accept �̺�Ʈ�� �� �����忡���� ��ٸ���.
		
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