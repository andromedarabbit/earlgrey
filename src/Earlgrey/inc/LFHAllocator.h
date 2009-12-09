#pragma once

class LowFragmentationHeapMemoryAllocator
{
public:
	typedef size_t size_type;

	LowFragmentationHeapMemoryAllocator()
	{
		// http://msdn2.microsoft.com/en-US/library/aa366750.aspx
		intptr_t	CrtHeapHandle	= _get_heap_handle();
		ULONG		EnableLFH		= 2;
		HeapSetInformation( (PVOID)CrtHeapHandle, HeapCompatibilityInformation, &EnableLFH, sizeof(EnableLFH) );
	}

	void* Alloc(size_type bytes)
	{
		return malloc(bytes);
	}

	inline void Free(void * ptr)
	{
		free(ptr);
	}
};