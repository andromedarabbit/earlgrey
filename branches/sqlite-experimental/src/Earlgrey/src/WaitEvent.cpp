#include "stdafx.h"
#include "WaitEvent.h"

#include "numeric_cast.hpp"
#include "Lock.h"

#pragma comment(lib, "Ws2_32.lib")

namespace Earlgrey {

	void WaitEventContainer::Add(HANDLE AcceptEvent, IWaitHandler* Handler)
	{
		scoped_lock_type Lock(_Lock);

		EARLGREY_ASSERT( _WaitEventList.size() <= WSA_MAXIMUM_WAIT_EVENTS );
		if (_WaitEventList.size() > WSA_MAXIMUM_WAIT_EVENTS)
		{
			// �� �̻� �̺�Ʈ�� �߰��� �� ����.
			return;
		}
		_WaitEventList.push_back( AcceptEvent );
		_WaitHandlerMap.insert( std::make_pair( AcceptEvent, Handler ) );
	}

	void WaitEventContainer::Remove(HANDLE AcceptEvent)
	{
		scoped_lock_type Lock(_Lock);
		HandleVectorType::const_iterator iter = std::find( _WaitEventList.begin(), _WaitEventList.end(), AcceptEvent );
		if (iter == _WaitEventList.end())
		{
			return;
		}
		_WaitHandlerMap.erase( AcceptEvent );
		_WaitEventList.erase( iter );
	}

	void WaitEventContainer::WaitEvents(DWORD WaitTime)
	{
		// TODO: lock-free�� �����ؾ� �Ѵ�.
		scoped_lock_type Lock(_Lock);
		DWORD EventSize = EARLGREY_NUMERIC_CAST<DWORD>( _WaitEventList.size() );
		if (0 == EventSize)
		{
			Sleep( WaitTime );
			return;
		}
		DWORD Index = ::WSAWaitForMultipleEvents( EventSize, &_WaitEventList[0], FALSE, WaitTime, FALSE );
		if (WSA_WAIT_EVENT_0 <= Index && Index < WSA_WAIT_EVENT_0 + EventSize)
		{
			Index -= WSA_WAIT_EVENT_0;

			IWaitHandler* Handler = _WaitHandlerMap[_WaitEventList[Index]];
			EARLGREY_ASSERT( Handler );
			if (Handler)
			{
				if (Handler->DoTask())
				{
					// true�� �����ϸ� �̺�Ʈ �ڵ��� �����Ѵ�.
					_WaitEventList.erase( _WaitEventList.begin() + Index );
				}
			}
		}
		else if (Index == WAIT_TIMEOUT)
		{		
		}
		else
		{
			// TODO: ����ó���ʿ�
		}
	}

}