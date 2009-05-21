#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"
#include "EarlgreyMath.h"

#include "MemoryBlock.h"


namespace Earlgrey
{

	//! \todo Ŭ���� �̸��� �ٲٸ� ���ڴ�.
	class MemoryManager : private Uncopyable
	{
		
	public:
		// static const double TWO = 2;
		typedef SuperMemoryBlock::size_type size_type;

		/*
		enum 
		{
			MAX_POOL_ALLOCATABLE_BYTES = Math::PowStatic<m_UpperLimitN>(2);
		};
		*/

		/*
		explicit MemoryManager(unsigned int lowerLimitN, unsigned int upperLimitN)
			: m_MinBytes(pow(TWO, lowerLimitN))
			: m_MaxBytes(pow(TWO, upperLimitN))
		{
			EARLGREY_ASSERT(Math::IsPowerOf2(m_MinBytes) == TRUE);
			EARLGREY_ASSERT(Math::IsPowerOf2(m_MaxBytes) == TRUE);


		}
		*/
		explicit MemoryManager(unsigned int maxBytes, unsigned int minBytes)
			: m_MinBytes(minBytes)
			, m_MaxBytes(maxBytes)
			// , m_SuperBlocks(
		{
			EARLGREY_ASSERT(Math::IsPowerOf2(m_MinBytes) == TRUE);
			EARLGREY_ASSERT(Math::IsPowerOf2(m_MaxBytes) == TRUE);
			EARLGREY_ASSERT(m_MinBytes >= m_MaxBytes);
			


		}


		void * Malloc(size_type bytes)
		{
			// EARLGREY_STATIC_ASSERT(EARLGREY_DEFAULT_PAGE_ALIGNMENT <= MAX_POOL_ALLOCATABLE_BYTES);

			// ����� ũ���� �޸� ����� �䱸�ϸ� alloc�� ���� �޴´�.
			// ��? ����ȭ ������ ������ ������ �����ϱ�!
			// if(bytes > MAX_POOL_ALLOCATABLE_BYTES) 
			{
				MemoryBlock * memoryBlock  = SuperMemoryBlock::AllocLargeObjectHeap(bytes, EARLGREY_DEFAULT_PAGE_ALIGNMENT);
				return memoryBlock->Data();
			}

			return NULL;
		}
		


	private:
		// unsigned short m_LowerLimitN;
		// unsigned short m_UpperLimitN;
		size_type m_MaxBytes;
		size_type m_MinBytes;

		SuperMemoryBlock ** m_SuperBlocks;


	};


}