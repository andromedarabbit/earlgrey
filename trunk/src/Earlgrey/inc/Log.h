#pragma once
#include "txstring.h"
#include "Uncopyable.h"

namespace Earlgrey
{
	class Uncopyable;

	//! \todo 임시 구현
	//! \ref http://recoverlee.tistory.com/9
	class Log : private Uncopyable
	{
	private:
		explicit Log();

	public:
		static _txstring ErrorMessage(DWORD errorCode, HMODULE source);
		static _txstring ErrorMessage(DWORD errorCode);
	};
}