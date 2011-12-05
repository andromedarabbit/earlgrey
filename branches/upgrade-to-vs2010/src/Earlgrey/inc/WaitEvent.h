#pragma once
#include <Loki/Singleton.h>

namespace Earlgrey {

	//class IWaitHandler 
	//{
	//	friend class WaitEventContainer;
	//public:
	//	virtual ~IWaitHandler() {}

	//protected:

	//	//! 이벤트가 완료된 후 처리할 hook method
	//	/*!
	//		\return true를 리턴하면 이벤트 핸들을 제거한다.
	//	*/
	//	virtual bool DoTask() = 0;
	//};

	////! Wait 이벤트 목록을 관리한다.
	///*!
	//	최대 WSA_MAXIMUM_WAIT_EVENTS(64) 개의 이벤트만 처리할 수 있으므로 그 이상의 Handle은 처리할 수 없다.
	//*/
	//class WaitEventContainer
	//{
	//	//! \todo vector에 노드 추가/삭제가 빈번하므로 xvector로 바꿔야 한다.
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