#pragma once

namespace Earlgrey
{
	typedef std::tr1::shared_ptr<void> StatePtr;

	typedef std::tr1::function<void(StatePtr)> TimerCallback;
	typedef std::tr1::shared_ptr<TimerCallback> TimerCallbackPtr;

}