#pragma once
#include "Uncopyable.h"

class Win32ServiceHelper
	: public Earlgrey::Uncopyable
{
private:
	explicit Win32ServiceHelper();
	~Win32ServiceHelper();

public:
	static BOOL RunByWin32Service();
};