#pragma once

namespace Earlgrey {

	class IWaitHandler 
	{
	public:
		virtual ~IWaitHandler() {}
		virtual void DoTask() = 0;
	};

	//! Wait 이벤트 목록을 관리한다.
	/*!
		최대 WSA_MAXIMUM_WAIT_EVENTS(64) 개의 이벤트만 처리할 수 있으므로 그 이상의 Handle은 처리할 수 없다.
	*/
	class WaitEventContainer
	{
		typedef std::vector<HANDLE> HandleVectorType;
	public:
		void Add(HANDLE AcceptEvent, IWaitHandler* Handler)
		{
			ScopedLock Lock(&_Lock);

			EARLGREY_ASSERT( _WaitEventList.size() <= WSA_MAXIMUM_WAIT_EVENTS );
			if (_WaitEventList.size() > WSA_MAXIMUM_WAIT_EVENTS)
			{
				// 더 이상 이벤트를 추가할 수 없다.
				return;
			}
			_WaitEventList.push_back( AcceptEvent );
			_WaitHandlerMap.insert( std::make_pair( AcceptEvent, Handler ) );
		}

		void Remove(HANDLE AcceptEvent)
		{
			ScopedLock Lock(&_Lock);
			HandleVectorType::const_iterator iter = std::find( _WaitEventList.begin(), _WaitEventList.end(), AcceptEvent );
			if (iter == _WaitEventList.end())
			{
				return;
			}
			_WaitHandlerMap.erase( AcceptEvent );
			_WaitEventList.erase( iter );
		}

		void WaitEvents(DWORD WaitTime = 100)
		{
			// TODO: lock-free로 구현해야 한다.
			ScopedLock Lock(&_Lock);
			DWORD EventSize = EARLGREY_NUMERIC_CAST<DWORD>( _WaitEventList.size() );
			if (0 == EventSize)
			{
				Sleep( WaitTime );
				return;
			}
			DWORD Result = ::WaitForMultipleObjects( EventSize, &_WaitEventList[0], FALSE, WaitTime );
			if (WAIT_OBJECT_0 <= Result && Result < WAIT_OBJECT_0 + EventSize)
			{
				IWaitHandler* Handler = _WaitHandlerMap[_WaitEventList[Result - WAIT_OBJECT_0]];
				EARLGREY_ASSERT( Handler );
				if (Handler)
				{
					Handler->DoTask();
				}
			}
			else if (Result == WAIT_TIMEOUT)
			{		
			}
			else
			{
				// TODO: 에러처리필요
			}
		}
	private:
		::Loki::Mutex _Lock;
		HandleVectorType _WaitEventList;
		std::map<HANDLE, IWaitHandler*> _WaitHandlerMap;
	};

	typedef ::Loki::SingletonHolder<WaitEventContainer> WaitEventContainerSingleton;

}