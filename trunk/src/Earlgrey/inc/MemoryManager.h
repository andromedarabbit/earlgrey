#pragma once
#include "SinglyList.h"

namespace Earlgrey
{
	class DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) MemoryBlockNode
	{
	public:

		
		inline void * Data()
		{
			return this + 1;
		}

		inline size_t Size() const 
		{
			return m_Bytes;
		}

	private:
		size_t m_Bytes;
	};


	class MemorySuperBlock // List of MemoryBlocks
	{
	public:
		enum 
		{ 
			// DEFAULT_CHUNK_SIZE = 256 * 1024 // 256 kb
		};

		explicit MemorySuperBlock(size_t chunkSize, size_t blockSize)
			: m_ChunkSize(chunkSize)
			, m_BlockSize(blockSize)
		{
		}

	private:
		size_t m_ChunkSize;
		size_t m_BlockSize;
	};


	class MemoryManager
	{

	};


}