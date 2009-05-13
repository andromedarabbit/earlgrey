#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"
// #include "Singleton.h"
#include "ThreadLocalSingleton.hpp"

#include <memory>

namespace Earlgrey
{
	class StackAllocator : private Uncopyable
	{
		// friend struct Loki::CreateUsingNew<StackAllocator>;
	
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

		inline void * malloc(size_type size, size_type alignment = DEFAULT_ALIGNMENT)
		{
			EARLGREY_ASSERT(size > 0);
			EARLGREY_ASSERT(alignment <= DEFAULT_ALIGNMENT);
			EARLGREY_ASSERT( (alignment & (alignment - 1)) == 0 );

			m_current_pos = (m_current_pos + (alignment - 1)) & ~(alignment-1);
			pointer memblock = (BYTE*) (m_buffer_begin + m_current_pos);
			m_current_pos += size;
			
			EARLGREY_ASSERT(memblock + m_current_pos < m_buffer_end);

			return memblock;
		}

		inline void free(void * memblock)
		{
			UNREFERENCED_PARAMETER(memblock);
			// do nothing
		}

		// \todo 임시 코드
		template <class T>
		struct CreateUsingNew
		{
			enum { DEFAULT_STACK_BYTES = 1024 * 1024 };

			static StackAllocator* Create()
			{ 
				return new StackAllocator(DEFAULT_STACK_BYTES); 
			}

			static void Destroy(StackAllocator* p)
			{ 
				delete p; 
			}
		};

	private:
		explicit StackAllocator(size_type bytes)
			: m_buffer_begin( static_cast<pointer>(_aligned_malloc(bytes, DEFAULT_ALIGNMENT)) )
			, m_buffer_end(m_buffer_begin + bytes)
			, m_current_pos(0)
		{
			EARLGREY_ASSERT(bytes > 0);
			EARLGREY_ASSERT(m_buffer_begin != 0);
			EARLGREY_ASSERT(m_buffer_begin < m_buffer_end);

		}

	private:
		pointer m_buffer_begin;
		pointer m_buffer_end;
		size_type m_current_pos;

	};

	
	typedef 
		Loki::SingletonHolder<StackAllocator, StackAllocator::CreateUsingNew, ThreadLocalLifetime, ThreadLocalModel> 
		gStackAllocator;

}