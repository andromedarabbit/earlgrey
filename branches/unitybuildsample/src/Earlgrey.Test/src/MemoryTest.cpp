#include "stdafx.h"

#include "MemoryAllocator.h"
#include "LFHAllocator.h"
#include "ThreadLocal.hpp"

Earlgrey::ThreadLocalValue<INT> ThreadId;

template<DWORD TMaxThreadCount>
class MemoryTestThreadInterface
{

public:
	const static INT MaxThreadCount = TMaxThreadCount; 
	const static INT InvalidThreadId = -1;

	static void SetCurrentThreadIndex(INT tid)
	{
		ThreadId = tid;
	}

	static INT ThreadIndex()
	{
		return ThreadId;
	}
};

//  Main idea came from Larson's paper "Memory Allocation for Long-Running
//  Server Applications". But major difference from his method is this tool
//  does not create new thread per request. Instead maintains fixed size of
//  worker thread pool for malloc/free operations. Memory contexts are created
//  as a linked list at the beginning and each thread get a random memory
//  context and perform random malloc/free operations on it for a test round.
//  If a thread finished a test round it enqueues the mem context back to 
//  the original linked list. All test threads will be running during the
//  test run time specified.
template<class MallocClass, DWORD NumberOfThread, DWORD ContextPerThread, DWORD BlocksPerContext>
class MemoryTest : public testing::Test 
{
	typedef MemoryTestThreadInterface<NumberOfThread> ThreadInterface;
protected:
	enum { NumberOfContexts = NumberOfThread * ContextPerThread };

	const static DWORD MAX_TEST_ROUNDS         = 100000;
	const static DWORD MAX_PER_CONTEXT_BLOCKS  = 100000;
	const static DWORD MAX_ALLOC_SIZE          = 100 * 1024 * 1024;  // 10 MB
	const static DWORD MAX_SLEEP_MS            = 60 * 1000;
	const static DWORD MAX_TEST_RUN_TIME       = 24 * 60 * 10 * 1000;   // 1 day

	MemoryTest() {}

	virtual void SetUp() 
	{
		TestMalloc = new MallocClass;

		testRunTime = 30 * 1000;
		testRounds = 2;
		minAllocSize = 0;
		maxAllocSize = 32*1024;
		randSeed = 1234;
		sleepMs = 0;

		srand(randSeed);
	}

	virtual void TearDown() 
	{
		delete TestMalloc;
		TestMalloc = NULL;
	}

	virtual MallocClass* Mallocator() 
	{
		return TestMalloc;
	}

	struct ThreadInfo 
	{
		DWORD        threadNo;
		DWORD        AllocCount;
		DWORD        FreeCount;
		DWORD        ContextCount;
		MemoryTest*  TestContext;
	};

	struct MemContext 
	{
		MemContext(DWORD InContextId = 0) : prev(NULL), next(NULL)
		{
			contextId = InContextId;
			memset(blkArray, 0, sizeof(blkArray));
			memset(blkSize, 0, sizeof(blkSize));
		}

		MemContext *	prev;
		MemContext *	next;

		DWORD   contextId;
		void*   blkArray[BlocksPerContext];
		DWORD   blkSize[BlocksPerContext];
	};

	MallocClass* TestMalloc;

	ThreadInfo ThreadInfoArray[NumberOfThread];
	CRITICAL_SECTION     mCritsect;
	MemContext*          mMemContextHead;
	MemContext*          mMemContextTail;
	HANDLE               mHeap;

	DWORD               NumberOfRunngingThread;
	HANDLE              RunTestWait;
	volatile bool       bStopTesting;

	static inline __int64 GetPerfCounters ()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return li.QuadPart;
	}

	static inline __int64 GetPerfFreq ()
	{
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		return li.QuadPart;
	}

	void InitializeMemContextQueue () 
	{
		EnterCriticalSection(&mCritsect);
		mMemContextHead = new MemContext();
		mMemContextTail = new MemContext();
		mMemContextHead->prev = NULL;
		mMemContextHead->next = mMemContextTail;
		mMemContextTail->prev = mMemContextHead;
		mMemContextTail->next = NULL;
		LeaveCriticalSection(&mCritsect);
	}

	void EnqueMemContext (MemContext * context) 
	{
		EnterCriticalSection(&mCritsect);
		MemContext* last = mMemContextTail->prev;
		last->next    = context;
		context->prev = last;
		context->next = mMemContextTail;
		mMemContextTail->prev = context;
		LeaveCriticalSection(&mCritsect);
	}

	MemContext * DequeMemContext () 
	{
		EnterCriticalSection(&mCritsect);
		MemContext* context = mMemContextHead->next;
		if (context->next != mMemContextTail) 
		{
			mMemContextHead->next = context->next;
			context->next->prev = mMemContextHead;
		}
		else 
		{
			context = NULL;
		}
		LeaveCriticalSection(&mCritsect);

		return context;
	}

	static DWORD WINAPI ThreadProc(void* lpParameter) 
	{
		ThreadInfo* threadInfo = static_cast<ThreadInfo*>(lpParameter);
		if(NULL == threadInfo) return 1;

		ThreadInterface::SetCurrentThreadIndex(threadInfo->threadNo);
		return threadInfo->TestContext->TestThreadProc();
	}

	DWORD WINAPI TestThreadProc() 
	{
		assert(ThreadInterface::ThreadIndex() != ThreadInterface::InvalidThreadId);
		assert(ThreadInterface::ThreadIndex() < ThreadInterface::MaxThreadCount);
		ThreadInfo* threadInfo = &ThreadInfoArray[ThreadInterface::ThreadIndex()];

		// Wait for start
		InterlockedIncrement((long*) &NumberOfRunngingThread);
		WaitForSingleObject(RunTestWait, INFINITE);

		while (!bStopTesting) 
		{
			// Dequeue a mem context
			MemContext * context = DequeMemContext();

			// if no memory context is available, try again after sleep
			if (!context) 
			{
				Sleep(sleepMs);
				continue;
			}

			threadInfo->ContextCount++;

			DWORD allocRange = maxAllocSize - minAllocSize;
			BYTE contextId = (BYTE)context->contextId;
			for (DWORD i = 0; i < testRounds * BlocksPerContext; i++) 
			{
				// Choose a random block
				DWORD victim     = rand() % BlocksPerContext;
				DWORD allocSize  = minAllocSize + rand() % allocRange;

				// Free it first if already allocated
				if (context->blkArray[victim]) 
				{
					if (context->blkSize[victim] && *(BYTE*)(context->blkArray[victim]) != contextId) 
					{
						char buff[128];
						sprintf_s(buff, sizeof(buff)/sizeof(*buff),
							"block-size=%d,contextId=%d,victim data=0x%x,allocSize=%d",
							context->blkSize[victim],
							contextId,
							*(BYTE*)context->blkArray[victim],
							allocSize
							);
						RecordProperty("Error", buff);
					}

					// Fill the memory with garbage value before free
					memset(context->blkArray[victim], 0xFE, context->blkSize[victim]);
					TestMalloc->Free(context->blkArray[victim]);
					context->blkArray[victim] = NULL;
					threadInfo->FreeCount++;
				}

				// Allocate random sized memory
				context->blkArray[victim] = TestMalloc->Alloc(allocSize);
				context->blkSize[victim]  = allocSize;

				// Write context Id to allocated memory
				memset(context->blkArray[victim], contextId, allocSize);

				threadInfo->AllocCount++;
			}

			// Enqueue the context
			EnqueMemContext(context);

			if (sleepMs)
				Sleep(sleepMs);
		}

		return 0;
	}

	void RunThreads () 
	{
		HANDLE handles[NumberOfThread];

		// Init global 
		NumberOfRunngingThread = 0;

		// create Wait event
		RunTestWait = CreateEvent(
			(LPSECURITY_ATTRIBUTES) 0,
			true,           // manual-reset
			false,          // initial state off
			(LPCTSTR) 0     // name
			);

		ResetEvent(RunTestWait);

		bStopTesting = false;

		ThreadId = ThreadInterface::InvalidThreadId;
		DWORD i;
		for (i = 0; i < NumberOfThread; i++) {
			ThreadInfoArray[i].threadNo        = i;
			ThreadInfoArray[i].AllocCount       = 0;
			ThreadInfoArray[i].FreeCount        = 0;
			ThreadInfoArray[i].ContextCount     = 0;
			ThreadInfoArray[i].TestContext      = this;

			DWORD threadId;
			handles[i] = (HANDLE) CreateThread(
				(LPSECURITY_ATTRIBUTES) 0,
				0,              // stack size
				MemoryTest::ThreadProc,
				(LPVOID)&ThreadInfoArray[i],      // argument
				0,
				&threadId
				);

			// Set higher priority for more accurate benchmark
			SetThreadPriority(handles[i], THREAD_PRIORITY_HIGHEST);
		}

		// Wait until all threads are ready
		while (NumberOfThread < NumberOfRunngingThread)
			Sleep(10);

		SetEvent(RunTestWait);

		// Run test for specified test time
		Sleep(testRunTime);

		// Stop testing
		bStopTesting = true;

		// Wait until all threads have terminated.
		WaitForMultipleObjects(
			NumberOfThread, 
			handles,
			TRUE,
			INFINITE
			);

		// Close thread handles
		for (i = 0; i < NumberOfThread; i++)
			CloseHandle(handles[i]);
	}

	void CreateMemContexts() 
	{
		for (DWORD i = 1; i <= NumberOfContexts; i++) 
		{
			MemContext * context = new MemContext(i);
			EnqueMemContext(context);
		}
	}

	void DestroyMemContexts () 
	{
		EnterCriticalSection(&mCritsect);
		MemContext* context = mMemContextHead->next;
		for (MemContext* next; context != mMemContextTail; context = next) {
			next = context->next;
			for (DWORD i = 0; i < BlocksPerContext; i++) {
				if (context->blkArray[i]) {
					TestMalloc->Free(context->blkArray[i]);
					context->blkArray[i] = NULL;
				}
			}
			delete(context);
		}
		LeaveCriticalSection(&mCritsect);
	}

	DWORD testRunTime;
	DWORD testRounds;
	DWORD minAllocSize;
	DWORD maxAllocSize;
	DWORD randSeed;
	DWORD sleepMs;

	void ExecuteMemoryWork()
	{
		InitializeCriticalSection(&mCritsect);
		InitializeMemContextQueue();

		CreateMemContexts();

		__int64 startCounter = GetPerfCounters();

		RunThreads();

		float totalRunTime = (float)(GetPerfCounters() - startCounter) / (float)GetPerfFreq();

		DestroyMemContexts();

		DeleteCriticalSection(&mCritsect);

		// Print per thread stats
		DWORD totalAllocs    = 0;
		DWORD totalFrees     = 0;
		DWORD totalContexts  = 0;
		for (DWORD i = 0; i < NumberOfThread; i++) {
			totalAllocs     += ThreadInfoArray[i].AllocCount;
			totalFrees      += ThreadInfoArray[i].FreeCount;
			totalContexts   += ThreadInfoArray[i].ContextCount;

			char prop[64];
			char val[128];
			sprintf_s(prop, sizeof(prop)/sizeof(*prop), "Thread-%u-Performance", ThreadInfoArray[i].threadNo);
			sprintf_s(val, sizeof(val)/sizeof(*val),
				"%u Allocs, %u Frees, %u Context Processed",
				ThreadInfoArray[i].AllocCount,
				ThreadInfoArray[i].FreeCount,
				ThreadInfoArray[i].ContextCount
				);
			RecordProperty(prop, val);
		}

		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);

		RecordProperty("Processors", systemInfo.dwNumberOfProcessors);
		RecordProperty("Test-Threads", NumberOfThread);
		RecordProperty("Test-Rounds", testRounds);
		RecordProperty("Blocks", BlocksPerContext);
		RecordProperty("Mem-Contexts", NumberOfContexts);
		RecordProperty("Min-Alloc-Size", minAllocSize);
		RecordProperty("Max-Alloc-Size", maxAllocSize);
		RecordProperty("Sleep-time-in-ms", sleepMs);
		RecordProperty("Total-threads", NumberOfThread);
		RecordProperty("Total-Alloc", totalAllocs);
		RecordProperty("Total-Frees", totalFrees);
		RecordProperty("Total-Context-Processed", totalContexts);
		RecordProperty("Operations-Per-Sec", (int)((totalAllocs + totalFrees) / totalRunTime));
	}
};

#define TEST_THREAD 4
#define CONTEXT_PER_THREAD 2
#define BLOCK_PER_CONTEXT 1024

using namespace Earlgrey;

typedef MemoryTest<DefaultMemoryAllocator, TEST_THREAD, CONTEXT_PER_THREAD, BLOCK_PER_CONTEXT> AnsiMemoryTestTemplate;
TEST_F(AnsiMemoryTestTemplate, AnsiMemoryPerformanceTest)
{
	ExecuteMemoryWork();
}

typedef MemoryTest<LowFragmentationHeapMemoryAllocator, TEST_THREAD, CONTEXT_PER_THREAD, BLOCK_PER_CONTEXT> LFHMemoryTestTemplate;
TEST_F(LFHMemoryTestTemplate, LFHMemoryPerformanceTest)
{
	ExecuteMemoryWork();
}

#if USE_MEMORY_ALLOCATOR_THREAD_BUILDING_BLOCK
typedef MemoryTest<ThreadBuildingBlockMemoryAllocator, TEST_THREAD, CONTEXT_PER_THREAD, BLOCK_PER_CONTEXT> TBBMemoryTestTemplate;
TEST_F(TBBMemoryTestTemplate, TBBMemoryPerformanceTest) 
{
	ExecuteMemoryWork();
}
#endif

typedef MemoryTest<ThreadLocalAllocator<SlabAllocator, MemoryTestThreadInterface<TEST_THREAD>>, TEST_THREAD, CONTEXT_PER_THREAD, BLOCK_PER_CONTEXT> TLSMemoryTestTemplate;
TEST_F(TLSMemoryTestTemplate, TLSMemoryPerformanceTest) 
{
	ExecuteMemoryWork();
}
