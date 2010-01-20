#pragma once 
#include "Uncopyable.h"

namespace Earlgrey
{
	class NoLock : private Uncopyable
	{
	public:
		inline explicit NoLock() {}
		~NoLock() {}

		inline void Lock() {}
		inline void UnLock() {}	
	};
}