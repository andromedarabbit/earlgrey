#pragma once
#include <Loki/Singleton.h>

namespace Earlgrey {

	//class IWaitHandler 
	//{
	//	friend class WaitEventContainer;
	//public:
	//	virtual ~IWaitHandler() {}

	//protected:

	//	//! �̺�Ʈ�� �Ϸ�� �� ó���� hook method
	//	/*!
	//		\return true�� �����ϸ� �̺�Ʈ �ڵ��� �����Ѵ�.
	//	*/
	//	virtual bool DoTask() = 0;
	//};

	////! Wait �̺�Ʈ ����� �����Ѵ�.
	///*!
	//	�ִ� WSA_MAXIMUM_WAIT_EVENTS(64) ���� �̺�Ʈ�� ó���� �� �����Ƿ� �� �̻��� Handle�� ó���� �� ����.
	//*/
	//class WaitEventContainer
	//{
	//	//! \todo vector�� ��� �߰�/������ ����ϹǷ� xvector�� �ٲ�� �Ѵ�.
	//	typedef std::vector<HANDLE> HandleVectorType;

	//	typedef Loki::Mutex mutex_type;
	//	typedef ScopedLock<mutex_type> scoped_lock_type;

	//public:
	//	void Add(HANDLE AcceptEvent, IWaitHandler* Handler);

	//	void Remove(HANDLE AcceptEvent);

	//	void WaitEvents(DWORD WaitTime = 100);

	//private:
	//	::Loki::Mutex _Lock;
	//	HandleVectorType _WaitEventList;
	//	std::map<HANDLE, IWaitHandler*> _WaitHandlerMap;
	//};

	//typedef ::Loki::SingletonHolder<WaitEventContainer> WaitEventContainerSingleton;

}