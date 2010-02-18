#pragma once
#include "Thread.h"
#include "ThreadRunnable.h"

class ServerService;

class Win32ServiceRunnable 
	: public Earlgrey::ThreadRunnable
{
public:
	explicit Win32ServiceRunnable();
	virtual ~Win32ServiceRunnable();

protected:
	virtual BOOL Init();
	virtual void Stop();
	virtual void Exit();

	virtual BOOL MeetsStopCondition() const;
	virtual DWORD DoTask();

private:
	BOOL m_meetsStopCondition;
};