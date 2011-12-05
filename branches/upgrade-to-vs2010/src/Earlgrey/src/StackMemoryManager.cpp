#include "stdafx.h"
#include "StackMemoryManager.h"

namespace Earlgrey
{
	StackMemoryManager::StackMemoryManager(size_type bytes, size_type alignment)
		: m_buffer_begin( static_cast<pointer>(_aligned_malloc(bytes, alignment)) )
		, m_buffer_end(m_buffer_begin + bytes)
		, m_current_pos(0)
		, m_alignment(alignment)
		, m_marking_count(BOTTOM_NO_OF_MARKING_COUNT)
	{
		EARLGREY_ASSERT(bytes > 0);
		EARLGREY_ASSERT(m_buffer_begin != 0);
		EARLGREY_ASSERT(m_buffer_begin < m_buffer_end);
		EARLGREY_ASSERT( Math::IsPowerOf2(m_alignment) == TRUE );

#ifdef _DEBUG
		SetDebugBit();
#endif
	}

	StackMemoryManager::~StackMemoryManager()
	{
		EARLGREY_ASSERT(m_marking_count == BOTTOM_NO_OF_MARKING_COUNT);
		_aligned_free(m_buffer_begin);
	}


	void * StackMemoryManager::malloc(size_type size)
	{
		EARLGREY_ASSERT(size >= 0);
		EARLGREY_ASSERT(m_marking_count > BOTTOM_NO_OF_MARKING_COUNT);

		EARLGREY_ASSERT(m_current_pos == Math::NewMemoryAlignmentOffset(m_alignment, m_current_pos));

		pointer memblock = static_cast<pointer> (m_buffer_begin + m_current_pos);

		size_type sizeNeeded = size;
		if(size == 0)
			sizeNeeded = 1;
		m_current_pos = Math::NewMemoryAlignmentOffset(m_alignment, m_current_pos + sizeNeeded);

		EARLGREY_ASSERT((m_buffer_begin + m_current_pos) <= m_buffer_end);

		return memblock;
	}


	void StackMemoryManager::free(void * memblock)
	{
		// DBG_UNREFERENCED_PARAMETER(memblock);
		if(memblock < m_buffer_begin || memblock > m_buffer_end)
			throw std::exception("not supported yet!");

		EARLGREY_ASSERT( (static_cast<pointer>(memblock) - m_buffer_begin) >= 0);
		m_current_pos = static_cast<pointer>(memblock) - m_buffer_begin;
	
		if(m_current_pos == 0) // resize
		{

		}			
	}

	void StackMemoryManager::free(void * memblock, size_type bytes)
	{		
		size_type memblockSize = Math::NewMemoryAlignmentOffset(m_alignment, bytes);

		EARLGREY_ASSERT(static_cast<pointer>(memblock) - m_buffer_begin + memblockSize <= m_current_pos);
		if(static_cast<pointer>(memblock) - m_buffer_begin + memblockSize == m_current_pos)
			return free(memblock);

		
		// memblockEnd < m_buffer_end

	}
}