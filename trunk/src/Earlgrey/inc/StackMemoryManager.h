#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"
#include "MemoryMath.h"
#include "ThreadLocalSingleton.hpp"

#include <memory>

#ifdef EARLGREY_UNIT_TEST
#include <gtest/gtest_prod.h>
#endif


namespace Earlgrey
{
	class StackMemoryManager : private Uncopyable
	{
		// friend struct Loki::CreateUsingNew<StackMemoryManager>;
		friend class StackAllocator;

#ifdef EARLGREY_UNIT_TEST
		friend class StackMemoryManagerTest;
		FRIEND_TEST(StackMemoryManagerTest, Alloc);
		FRIEND_TEST(StackMemoryManagerTest, AllocNotWithAlignmentSize);
#endif

	public:
		typedef size_t    size_type;
		typedef ptrdiff_t difference_type;

		typedef BYTE              value_type;
		typedef BYTE*             pointer;
		typedef const BYTE*       const_pointer;
		typedef BYTE&             reference;
		typedef const BYTE&       const_reference;

		static const size_type DEFAULT_ALIGNMENT = 64;

		inline size_type capacity() const
		{
			return m_buffer_end - m_buffer_begin;
		}

		// \todo 임시 코드
		template <class T>
		struct CreateUsingNew
		{
			enum { DEFAULT_STACK_BYTES = 1024 * 1024 };

			static StackMemoryManager* Create()
			{ 
				return new StackMemoryManager(DEFAULT_STACK_BYTES); 
			}

			static void Destroy(StackMemoryManager* p)
			{ 
				delete p; 
			}
		};

	private:
		explicit StackMemoryManager(size_type bytes)
			: m_buffer_begin( static_cast<pointer>(_aligned_malloc(bytes, DEFAULT_ALIGNMENT)) )
			, m_buffer_end(m_buffer_begin + bytes)
			, m_current_pos(0)
			, m_marking_count(BOTTOM_NO_OF_MARKING_COUNT)
		{
			EARLGREY_ASSERT(bytes > 0);
			EARLGREY_ASSERT(m_buffer_begin != 0);
			EARLGREY_ASSERT(m_buffer_begin < m_buffer_end);

#ifdef _DEBUG
			SetDebugBit();
#endif
		}

		~StackMemoryManager()
		{
			EARLGREY_ASSERT(m_marking_count == BOTTOM_NO_OF_MARKING_COUNT);
			_aligned_free(m_buffer_begin);
		}

		inline void * malloc(size_type size, size_type alignment = DEFAULT_ALIGNMENT)
		{
			EARLGREY_ASSERT(size > 0);
			EARLGREY_ASSERT(alignment <= DEFAULT_ALIGNMENT);
			// EARLGREY_ASSERT( (alignment & (alignment - 1)) == 0 );
			EARLGREY_ASSERT( Math::IsPowerOf2(alignment) == TRUE );
			EARLGREY_ASSERT(m_marking_count > BOTTOM_NO_OF_MARKING_COUNT);

			// m_current_pos = (m_current_pos + (alignment - 1)) & ~(alignment-1);
			m_current_pos = Math::NewMemoryAligmentOffset(alignment, m_current_pos);
			pointer memblock = (BYTE*) (m_buffer_begin + m_current_pos);
			m_current_pos += size;

			EARLGREY_ASSERT(memblock + m_current_pos < m_buffer_end);

			return memblock;
		}

		inline void free(void * memblock)
		{
			// EARLGREY_ASSERT(m_marking_count > BOTTOM_NO_OF_MARKING_COUNT);

			UNREFERENCED_PARAMETER(memblock);
			// do nothing
		}

		inline void Mark() // StackMemMark를 생성해야 메모리 할당 및 해제 권한을 얻는다.
		{
			EARLGREY_ASSERT(m_marking_count >= BOTTOM_NO_OF_MARKING_COUNT);

			m_marking_count++;			
		}

		inline void Unmark(size_type pos) // 사실상 Unmark가 free 역할을 한다.
		{
			EARLGREY_ASSERT(m_marking_count > BOTTOM_NO_OF_MARKING_COUNT);
			EARLGREY_ASSERT(pos <= m_current_pos);

			m_marking_count--;
			m_current_pos = pos;

#ifdef _DEBUG
			SetDebugBit();			

			if(m_marking_count == BOTTOM_NO_OF_MARKING_COUNT)
			{
				EARLGREY_ASSERT(m_current_pos == 0);
			}
#endif
		}


#ifdef _DEBUG
		inline void SetDebugBit()
		{
			pointer empty_space_begin = m_buffer_begin + m_current_pos;
			size_type count = m_buffer_end - empty_space_begin;
			if(count == 0)
				return;

			memset(empty_space_begin, UINT_MAX, count);
		}
#endif

	private:
		enum { BOTTOM_NO_OF_MARKING_COUNT = -1};

		pointer m_buffer_begin;
		pointer m_buffer_end;
		size_type m_current_pos;

		INT32 m_marking_count;
	};


	typedef 
		Loki::SingletonHolder<StackMemoryManager, StackMemoryManager::CreateUsingNew, ThreadLocalLifetime, ThreadLocalThreaded> 
		gStackMemoryManager;

}