#pragma once
#include "StlCustom.h"
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
		static _txstring LastErrorMessage(DWORD lastErrorCode);
	};
}