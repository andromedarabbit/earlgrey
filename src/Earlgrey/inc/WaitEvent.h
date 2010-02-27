#pragma once

namespace Earlgrey {

	class IWaitHandler 
	{
	public:
		virtual ~IWaitHandler() {}
		virtual void DoTask() = 0;
	};

	//! Wait �̺�Ʈ ����� �����Ѵ�.
	/*!
		�ִ� WSA_MAXIMUM_WAIT_EVENTS(64) ���� �̺�Ʈ�� ó���� �� �����Ƿ� �� �̻��� Handle�� ó���� �� ����.
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
				// �� �̻� �̺�Ʈ�� �߰��� �� ����.
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
			// TODO: lock-free�� �����ؾ� �Ѵ�.
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
				// TODO: ����ó���ʿ�
			}
		}
	private:
		::Loki::Mutex _Lock;
		HandleVectorType _WaitEventList;
		std::map<HANDLE, IWaitHandler*> _WaitHandlerMap;
	};

	typedef ::Loki::SingletonHolder<WaitEventContainer> WaitEventContainerSingleton;

}