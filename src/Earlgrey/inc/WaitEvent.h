#pragma once
#include <Loki/Singleton.h>

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
		void Add(HANDLE AcceptEvent, IWaitHandler* Handler);

		void Remove(HANDLE AcceptEvent);

		void WaitEvents(DWORD WaitTime = 100);

	private:
		::Loki::Mutex _Lock;
		HandleVectorType _WaitEventList;
		std::map<HANDLE, IWaitHandler*> _WaitHandlerMap;
	};

	typedef ::Loki::SingletonHolder<WaitEventContainer> WaitEventContainerSingleton;

}