#include "stdafx.h"
#include "StackAllocator.h"

namespace Earlgrey
{
	void *Malloca(const size_t size)
	{
		return gStackMemoryManager::Instance().malloc(size);
	}

	void Freea(void *memblock)
	{
		gStackMemoryManager::Instance().free(memblock);
	}


}