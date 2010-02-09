#pragma once 
#include "Uncopyable.h"
#include "ScopedLock.h"

#include <Loki/Threads.h>

namespace Earlgrey
{
	typedef TScopedLock<Loki::Mutex> ScopedLock; // Loki::Mutex == CriticalSection
}

