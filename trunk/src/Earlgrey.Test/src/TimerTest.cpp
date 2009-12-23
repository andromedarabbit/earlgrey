#include "stdafx.h"
#include "Timer.h"

namespace Earlgrey
{
	namespace Test
	{
		// typedef std::tr1::shared_ptr<void> StatePtr;
		// typedef std::tr1::function<void(StatePtr)> TimerCallback;
		void IncrementStateValue(StatePtr state)
		{
			int* stateValue = reinterpret_cast<int>(state.get());

		}

		TEST(TimerTest, TempName)
		{
			int stateValue = 7;
			StatePtr state(&stateValue);

			TimerCallback callback(&IncrementStateValue);

			// TimerCallback callback()
			//Timer timer()
		}
	}
}