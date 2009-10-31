#pragma once

namespace Earlgrey
{
	//
	typedef DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) SLIST_ENTRY SinglyListEntry;
	typedef DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) SLIST_HEADER SinglyListHead;

	// Initializes the head of a singly linked list.
	inline void InitializeSinglyListHead(__inout SinglyListHead* head)
	{
		InitializeSListHead(head);
	}

	// Removes an item from the front of a singly linked list
	inline SinglyListEntry* InterlockedPopEntrySinglyList(__inout SinglyListHead* head)
	{
		return InterlockedPopEntrySList(head);
	}

	// Inserts an item at the front of a singly linked list.
	inline SinglyListEntry* InterlockedPushEntrySinglyList(__inout SinglyListHead* head, __inout SinglyListEntry* entry)
	{
		return InterlockedPushEntrySList(head, entry);
	}	

	// Flushes the entire list of items in a singly linked list.
	inline SinglyListEntry* InterlockedFlushSinglyList(__inout SinglyListHead* head)
	{
		return InterlockedFlushSList(head);
	}

	// Retrieves the number of entries in the specified singly linked list.
	inline USHORT QueryDepthSinglyList(__inout SinglyListHead* head)
	{
		return QueryDepthSList(head);
	}

}