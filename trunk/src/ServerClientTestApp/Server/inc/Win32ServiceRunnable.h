#pragma once
#include "Thread.h"

class ServerService;

//! \todo Win32ServiceRunnable로 이름 바꾸기 
class Win32ServiceRunnable 
	: public Earlgrey::IRunnable
{
public:
	explicit Win32ServiceRunnable(std::tr1::shared_ptr<ServerService> service);

	virtual ~Win32ServiceRunnable();

	virtual BOOL Init();
	virtual DWORD Run();
	virtual void Stop();

private:
	std::tr1::shared_ptr<ServerService> m_service;
	virtual void Exit();
};