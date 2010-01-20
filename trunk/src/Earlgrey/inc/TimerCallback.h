#pragma once
#include <functional>
namespace Earlgrey
{
	//! \todo rename to TimerState
	typedef std::tr1::shared_ptr<void> StatePtr;

	typedef std::tr1::function<void(StatePtr)> TimerCallback;
	// typedef std::tr1::shared_ptr<TimerCallback> TimerCallbackPtr;

}