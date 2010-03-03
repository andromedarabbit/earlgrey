#pragma once
#include <Loki/Singleton.h>

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