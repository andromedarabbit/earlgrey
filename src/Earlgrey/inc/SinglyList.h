#pragma once

namespace Earlgrey
{
	// On 64-bit platforms, SLIST_ENTRY structures must be 16-byte aligned. 
	// Drivers can use DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) to ensure the proper alignment of SLIST_ENTRY. 
	// typedef SLIST_ENTRY SinglyListEntry;
	// typedef SLIST_HEADER SinglyListHead;
	typedef DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) SLIST_ENTRY SinglyListEntry;
	typedef DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) SLIST_HEADER SinglyListHead;


	inline void InitializeSinglyListHead(__inout SinglyListHead* head)
	{
		InitializeSListHead(head);
	}

	inline SinglyListEntry* InterlockedPushEntrySinglyList(__inout SinglyListHead* head, __inout SinglyListEntry* entry)
	{
		// The return value is the previous first item in the list. If the list was previously empty, the return value is NULL.
		return InterlockedPushEntrySList(head, entry);
	}


	inline SinglyListEntry* InterlockedPushEntrySinglyList(__inout SinglyListHead* head)
	{
		// The return value is a pointer to the item removed from the list. If the list is empty, the return value is NULL.
		return InterlockedPopEntrySList(head);
	}

}