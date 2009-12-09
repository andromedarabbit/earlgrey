#pragma once

#if USE_MEMORY_ALLOCATOR_THREAD_BUILDING_BLOCK
#include <tbb/scalable_allocator.h>

class ThreadBuildingBlockMemoryAllocator
{
public:
	
	void* Alloc(size_type bytes)
	{
		return scalable_malloc(bytes);
	}
	
	inline void Free(void * ptr)
	{
		scalable_free(ptr);
	}
};
#endif