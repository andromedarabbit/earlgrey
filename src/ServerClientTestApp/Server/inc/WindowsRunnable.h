#pragma once
#include "Thread.h"


class WindowsRunnable 
	: public Earlgrey::IRunnable
{
public:
	WindowsRunnable() {}
	virtual ~WindowsRunnable() {}

	virtual BOOL Init(){return TRUE;}
	virtual DWORD Run()
	{
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{

		}
		return msg.wParam;
	}
	virtual void Stop() {}
	virtual void Exit() {}
};