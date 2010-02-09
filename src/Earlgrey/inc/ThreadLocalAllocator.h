#pragma once

#include <memory>
#include "MemoryMath.h"
#include "MemoryBlock.h"
#include "Thread.h"
#include "SlabAllocator.h"

 namespace Earlgrey
 {
	 class DefaultThreadInterface
	 {
	 public:
		 const static INT MaxThreadCount = MAX_IO_THREADS; 
		 const static INT InvalidThreadId = INVALID_THREAD_ID;
		 static INT ThreadIndex()
		 {
			 return (Thread::CurrentThread() == NULL)? InvalidThreadId: Thread::CurrentThread()->ThreadId();
		 }
	 };

 	template<class Allocator, class ThreadInterface>
 	class ThreadLocalAllocator
 	{
 	public:
 
 		explicit ThreadLocalAllocator()
 		{
 			 // 완전히 general 한 allocator를 쓰지는 못하고.. 껍데기라도 SetGlobalPool 인터페이스를 구현하고 있어야 컴파일이 됨 
 			EARLGREY_ASSERT(m_ExtraAllocator.GetRealAllocator() != NULL);
 			m_ExtraAllocator.GetRealAllocator()->SetGlobalPool(&ThreadGlobalPool);
			for(ThreadIdType i=0; i<ThreadInterface::MaxThreadCount; ++i)
 			{
 				m_ThreadLocalPool[i].SetGlobalPool(&ThreadGlobalPool);
 			}		
 		}
 
 		inline void* Alloc(typename Allocator::size_type bytes)
 		{
			INT tid = ThreadInterface::ThreadIndex();
			return IsValidThreadIndex(tid)? m_ThreadLocalPool[tid].Alloc(bytes) : m_ExtraAllocator.Alloc(bytes);
 		}
 		inline void* Realloc(void* ptr, DWORD bytes)
 		{
			INT tid = ThreadInterface::ThreadIndex();
			return IsValidThreadIndex(tid)? m_ThreadLocalPool[tid].Realloc(ptr, bytes) : m_ExtraAllocator.Realloc(ptr, bytes);
 		}
 		inline void Free(void * ptr)
 		{
			INT tid = ThreadInterface::ThreadIndex();
			return IsValidThreadIndex(tid)? m_ThreadLocalPool[tid].Free(ptr) : m_ExtraAllocator.Free(ptr);
 		}
 
 		void DumpLog(/* Output */)
 		{
 			INT PhysicalCurrentBytes = 0;
 			INT UsedCurrentBytes = 0;
 			INT UsedPeakBytes = 0;
 			INT CurrentAllocs = 0;
 			DWORD TotalAllocs = 0;
 			DOUBLE MemTime = 0;
 
 			//Output.PrintLine( _T("Memory Allocation Status") );
 			INT TotalAllocCount = 0;
 			INT TotalPoolCount  = 0;
 			INT TotalGlobalPoolCount = 0;
 			INT TotalMemUsed    = 0;
 			INT TotalMemPooled   = 0;
 			INT TotalPhysicalUsed = 0;
 			DWORD TotalAccumulated = 0;
 
 			DWORD BeginThreadId = 0;
 			DWORD EndThreadId = GIOWorkerCount;
 			BOOL IsThreadIdSpecified = IsValidThreadId(ThreadId);// ThreadId < MAX_IO_THREADS;
 			if(IsThreadIdSpecified)
 			{
 				EndThreadId  = (BeginThreadId = ThreadId) + 1;
 				//Output.PrintLine( _T("Thread %d's statistic"), ThreadId );
 			}
 
 			//Output.PrintLine( _T("Memory block information size : %d bytes"), sizeof(MemoryBlock));
 			//Output.PrintLine( _T("Block Size Cur Allocs Num Pools (Global) Mem Used Mem Pooled Physical Used Efficiency Total Allocs") );
 			//Output.PrintLine( _T("---------- ---------- ------------------ -------- ---------- ------------- ---------- ------------") );
 
 			for(DWORD i=0; i<FMallocSlab::POOL_COUNT; ++i )
 			{
 				INT BlockSize = FMallocSlab::BucketSize(i)-sizeof(MemoryBlock);
 				if(BlockSize <= 0) continue;
 
 				INT ThreadSumAllocCount = 0;
 				INT ThreadSumPoolCount  = 0;
 				INT GlobalPoolCount = ThreadGlobalPool.ChunkQueue(i)->BlockCount();
 				INT ThreadSumMemUsed = 0;
 				INT ThreadSumMemPooled = 0;
 				INT ThreadSumPhysicalUsed = 0;
 				DWORD ThreadSumTotalAlloc = 0;
 
 				for (DWORD k=BeginThreadId; k<EndThreadId ; ++k) 
 				{
 					ThreadSumPoolCount += ThreadLocalPool[k].PoolCount(i);
 					ThreadSumAllocCount += ThreadLocalPool[k].AllocCount(i);
 					ThreadSumPhysicalUsed += ThreadLocalPool[k].PhysicalAllocCount(i);
 					ThreadSumTotalAlloc += ThreadLocalPool[k].TotalAllocCount(i);
 #ifdef DEBUG_MEMORY_EFFICIENCY
 					ThreadSumMemUsed += ThreadLocalPool[k].ActualMemUse(i);
 #endif
 				}
 
 				if(FALSE == IsThreadIdSpecified)
 				{
 					ThreadSumPoolCount += ThreadLocalPool[ExtraMemoryPoolIndex].PoolCount(i);
 					ThreadSumAllocCount += ThreadLocalPool[ExtraMemoryPoolIndex].AllocCount(i);
 					ThreadSumPhysicalUsed += ThreadLocalPool[ExtraMemoryPoolIndex].PhysicalAllocCount(i);
 					ThreadSumTotalAlloc += ThreadLocalPool[ExtraMemoryPoolIndex].TotalAllocCount(i);		
 #ifdef DEBUG_MEMORY_EFFICIENCY
 					ThreadSumMemUsed += ThreadLocalPool[ExtraMemoryPoolIndex].ActualMemUse(i);
 #endif
 				}
 
 #ifndef DEBUG_MEMORY_EFFICIENCY
 				ThreadSumMemUsed = ThreadSumAllocCount * BlockSize;
 #endif
 				ThreadSumMemPooled = ThreadSumPoolCount * BlockSize;
 
 				TotalAllocCount += ThreadSumAllocCount;
 				TotalPoolCount  += ThreadSumPoolCount;
 				TotalGlobalPoolCount += GlobalPoolCount;
 				TotalMemUsed += ThreadSumMemUsed;
 				TotalMemPooled += ThreadSumMemPooled;
 				TotalPhysicalUsed += ThreadSumPhysicalUsed;
 				TotalAccumulated += ThreadSumTotalAlloc;
 
 					//Output.PrintLine(
 					//	TEXT("%10i %10s %9s (%6s) %8s %10s %13s % 9.2f%% %12s"),
 					//	BlockSize,
 					//	*FNumericPrefixes<INT, FMetricPrefixes>(ThreadSumAllocCount).ToString(),
 					//	*FNumericPrefixes<INT, FMetricPrefixes>(ThreadSumPoolCount).ToString(),
 					//	*FNumericPrefixes<INT, FMetricPrefixes>(GlobalPoolCount).ToString(),
 					//	*FNumericPrefixes<INT, FBinaryPrefixes>(ThreadSumMemUsed).ToString(),
 					//	*FNumericPrefixes<INT, FBinaryPrefixes>(ThreadSumMemPooled).ToString(),
 					//	*FNumericPrefixes<INT, FBinaryPrefixes>(ThreadSumPhysicalUsed).ToString(),
 					//	ThreadSumPhysicalUsed ? 100.0 * (ThreadSumMemUsed + ThreadSumMemPooled) / ThreadSumPhysicalUsed : 100.0,
 					//	*FNumericPrefixes<DWORD, FMetricPrefixes>(ThreadSumTotalAlloc).ToString()
 					//	);
 			}
 
 			//Output.PrintLine(
 			//	TEXT("BlockTotal %10s %9s (%6s) %8s %10s %13s % 9.2f%% %12s"),
 			//	*FNumericPrefixes<INT, FMetricPrefixes>(TotalAllocCount).ToString(),
 			//	*FNumericPrefixes<INT, FMetricPrefixes>(TotalPoolCount).ToString(),
 			//	*FNumericPrefixes<INT, FMetricPrefixes>(TotalGlobalPoolCount).ToString(),
 			//	*FNumericPrefixes<INT, FBinaryPrefixes>(TotalMemUsed).ToString(),
 			//	*FNumericPrefixes<INT, FBinaryPrefixes>(TotalMemPooled).ToString(),
 			//	*FNumericPrefixes<INT, FBinaryPrefixes>(TotalPhysicalUsed).ToString(),
 			//	TotalPhysicalUsed ? 100.0 * (TotalMemUsed + TotalMemPooled) / TotalPhysicalUsed : 100.0,
 			//	*FNumericPrefixes<DWORD, FMetricPrefixes>(TotalAccumulated).ToString()
 			//	);
 		}
 
 	private:
		static BOOL IsValidThreadIndex(INT ThreadIndex)
		{
			return (ThreadInterface::InvalidThreadId != ThreadIndex) && (ThreadIndex < ThreadInterface::MaxThreadCount);
		}

		Allocator m_ThreadLocalPool[ThreadInterface::MaxThreadCount];
 		MemoryAllocatorThreadSafeProxy<Allocator> m_ExtraAllocator;
 		MemoryBlockChunkPool ThreadGlobalPool;
 	};
 }