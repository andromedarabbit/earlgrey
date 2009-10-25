#pragma once

#include <memory>
#include "MemoryMath.h"
#include "MemoryBlock.h"
#include "LockfreeQueue.hpp"

namespace Earlgrey
{
	template<class Allocator>
	class ThreadLocalAllocator
	{
	public:
		// TODO: 코드 돌아는 가게 하려고 써놓은 임시 변수. Thread index 제대로 setting 되면 날려야 함
		const static DWORD ThreadId = std::numeric_limits<DWORD>::max();
		const static DWORD MaxIoThread = 1;

		ThreadLocalAllocator()
		{
			 // 완전히 general 한 allocator를 쓰지는 못하고.. 껍데기라도 SetGlobalPool 인터페이스를 구현하고 있어야 컴파일이 됨 
			EARLGREY_ASSERT(m_ExtraAllocator.GetRealAllocator() != NULL);
			m_ExtraAllocator.GetRealAllocator()->SetGlobalPool(&ThreadGlobalPool);
			for(DWORD i=0; i<MaxIoThread; ++i)
			{
				m_ThreadLocalPool[i].SetGlobalPool(&ThreadGlobalPool);
			}		
		}

		inline void* Alloc(typename Allocator::size_type bytes)
		{
			return (ThreadId < MaxIoThread) ? &m_ThreadLocalPool[ThreadId].Alloc(bytes) : m_ExtraAllocator.Alloc(bytes);
		}
		inline void* Realloc(void* ptr, DWORD bytes)
		{
			return (ThreadId < MaxIoThread) ? &m_ThreadLocalPool[ThreadId].Realloc(ptr, bytes) : m_ExtraAllocator.Realloc(ptr, bytes);
		}
		inline void Free(void * ptr)
		{
			return (ThreadId < MaxIoThread) ? &m_ThreadLocalPool[ThreadId].Free(ptr) : m_ExtraAllocator.Free(ptr);
		}

		//void DumpAllocs( Archive& Ar )
		//{
		//	// Print per thread stats
		//	INT PhysicalCurrentBytes = 0;
		//	INT UsedCurrentBytes = 0;
		//	INT UsedPeakBytes = 0;
		//	INT CurrentAllocs = 0;
		//	DWORD TotalAllocs = 0;
		//	DOUBLE MemTime = 0;

		//	for (DWORD i = 0; i < GIOWorkerCount; i++) {
		//		PhysicalCurrentBytes += ThreadLocalPool[i].PhysicalCurrentBytes();
		//		UsedCurrentBytes += ThreadLocalPool[i].UsedCurrentBytes();
		//		UsedPeakBytes += ThreadLocalPool[i].UsedPeakBytes();
		//		CurrentAllocs += ThreadLocalPool[i].CurrentAllocs();
		//		TotalAllocs += ThreadLocalPool[i].TotalAllocs();
		//		MemTime += ThreadLocalPool[i].MemTime();
		//	}

		//	Ar.Logf( _T("Memory Allocation Status") );
		//	Ar.Logf( _T("Curr Memory % 5.3fM / % 5.3fM"), UsedCurrentBytes/1024.0/1024.0, PhysicalCurrentBytes/1024.0/1024.0 );
		//	Ar.Logf( _T("Peak Memory % 5.3fM / % 5.3fM"), UsedPeakBytes   /1024.0/1024.0, PhysicalCurrentBytes/1024.0/1024.0 );
		//	Ar.Logf( _T("Allocs      % 6i Current / % 6i Total"), CurrentAllocs, TotalAllocs );
		//	Ar.Logf( _T("Seconds     % 5.3f"), MemTime );
		//	Ar.Logf( _T("MSec/Allc   % 5.5f"), 1000.0 * MemTime / TotalAllocs );

		//	if(FALSE == bSummaryOnly)
		//	{
		//		Ar.Logf( _T("Block Size Num Pools Cur Allocs Total Allocs Mem Used Mem Waste Efficiency") );
		//		Ar.Logf( _T("---------- --------- ---------- ------------ -------- --------- ----------") );
		//		INT TotalPoolCount  = 0;
		//		INT TotalAllocCount = 0;
		//		INT TotalMemUsed    = 0;
		//		INT TotalMemWaste   = 0;
		//		for( INT i=0; i<POOL_COUNT; i++ )
		//		{
		//			Ar.Logf
		//				(
		//				TEXT("% 10i % 9i % 10i % 11iK % 7iK % 8iK % 9.2f%%"),
		//				Table->BlockSize,
		//				PoolCount,
		//				AllocCount,
		//				0,
		//				MemUsed /1024,
		//				MemWaste/1024,
		//				MemUsed ? 100.0 * (MemUsed-MemWaste) / MemUsed : 100.0
		//				);
		//			TotalPoolCount  += PoolCount;
		//			TotalAllocCount += AllocCount;
		//			TotalMemUsed    += MemUsed;
		//			TotalMemWaste   += MemWaste;
		//		}
		//		Ar.Logf
		//			(
		//			TEXT("BlkOverall % 9i % 10i % 11iK % 7iK % 8iK % 9.2f%%"),
		//			TotalPoolCount,
		//			TotalAllocCount,
		//			0,
		//			TotalMemUsed /1024,
		//			TotalMemWaste/1024,
		//			TotalMemUsed ? 100.0 * (TotalMemUsed-TotalMemWaste) / TotalMemUsed : 100.0
		//		);
		//	}
		//}

		template <class T> 
		struct CreateUsingNew
		{
			static T* Create() { return new T(); }
			static void Destroy(T* p) { delete p; }
		};

	private:
		Allocator m_ThreadLocalPool[MaxIoThread];
		MemoryAllocatorThreadSafeProxy<Allocator> m_ExtraAllocator;
		MemoryBlockChunkPool ThreadGlobalPool;
	};
}