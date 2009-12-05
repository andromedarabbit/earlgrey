#pragma once

namespace Earlgrey
{
	/*
	class Uncopyable;

	class TimerCallback : private Uncopyable
	{
	};
	*/

	typedef std::tr1::function<void(void*)> TimerCallback;
}