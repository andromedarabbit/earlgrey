#pragma once
#include "Thread.h"
#include "ThreadRunnable.h"

class ServerService;

class Win32ServiceRunnable 
	: public Earlgrey::ThreadRunnable
{
public:
	explicit Win32ServiceRunnable(HANDLE stopHandle);

	virtual ~Win32ServiceRunnable();

protected:
	virtual BOOL Init();
	virtual void Stop();
	virtual void Exit();

	virtual BOOL MeetsStopCondition() const;
	virtual DWORD DoTask();

private:
	HANDLE m_stopHandle;
};