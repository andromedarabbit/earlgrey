#pragma once
#include "txstring.h"
#include "Uncopyable.h"

namespace Earlgrey
{
	class Uncopyable;

	//! \todo �ӽ� ����
	class Log : private Uncopyable
	{
	private:
		explicit Log();

	public:
		static _txstring ErrorMessage(DWORD errorCode, HMODULE source);
		static _txstring ErrorMessage(DWORD errorCode);

	// private:
		// __declspec(thread) static TCHAR msgBuf[1024 * 2];
	};
}