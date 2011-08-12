#pragma once
#include "Uncopyable.h"

namespace Earlgrey
{
	class IRunnable
	{
	public:
		virtual ~IRunnable() {}
		virtual DWORD Run() = NULL;
	};

	//! \note �� Ŭ������ ���� ���Ǵ� ������?
	class RunnableBuilder : private Uncopyable
	{
	private:
		explicit RunnableBuilder();

	public:
		static std::tr1::shared_ptr<IRunnable> NewRunnable(IRunnable* runnable) {
			return std::tr1::shared_ptr<IRunnable> (runnable);
		}
	};

}