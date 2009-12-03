#pragma once
#include "Thread.h"

class ServerService;

//! \todo Win32ServiceRunnable�� �̸� �ٲٱ� 
class WindowsRunnable 
	: public Earlgrey::IRunnable
{
public:
	explicit WindowsRunnable(std::tr1::shared_ptr<ServerService> service);

	virtual ~WindowsRunnable();

	virtual BOOL Init();
	virtual DWORD Run();
	virtual void Stop();

private:
	std::tr1::shared_ptr<ServerService> m_service;
	virtual void Exit();
};