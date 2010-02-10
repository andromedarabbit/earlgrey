#include "stdafx.h"
#include "Application.h"
#include "Executor.h"
#include "Environment.h"
#include "DefaultAppSettings.h"

namespace Earlgrey
{
	namespace Test
	{
		class ExecutorTaskRunnable : public IRunnable {

		public:
			ExecutorTaskRunnable(const long MaxRunningCount) : MaxRunningCount_(MaxRunningCount) {}
			static long RunningCount;

			virtual ~ExecutorTaskRunnable()
			{
				InterlockedIncrement(&DestructorCount_);
			}

			virtual DWORD Run() 
			{
				long Count = InterlockedIncrement(&RunningCount);

				if (Count == MaxRunningCount_) {
					EARLGREY_VERIFY(Waiter_ != INVALID_HANDLE_VALUE);
					SetEvent(Waiter_);
				}

				return EXIT_SUCCESS;
			};		

			static void WaitFor() {
				EARLGREY_VERIFY(Waiter_ != INVALID_HANDLE_VALUE);
				DWORD Success = WaitForSingleObject(Waiter_, INFINITE);
				EARLGREY_VERIFY(Success == WAIT_OBJECT_0); 
			}

		public:
			long MaxRunningCount_;
			static HANDLE Waiter_;
			static long DestructorCount_;

		};

		long ExecutorTaskRunnable::RunningCount = 0;
		HANDLE ExecutorTaskRunnable::Waiter_ = INVALID_HANDLE_VALUE; // CreateEvent(NULL, TRUE, FALSE, NULL);
		long ExecutorTaskRunnable::DestructorCount_ = 0;


		class ExecutorTest : public ::testing::Test {
		protected:
			// You can define per-test set-up and tear-down logic as usual.
			virtual void SetUp() { 
				ExecutorTaskRunnable::RunningCount = 0;
				ExecutorTaskRunnable::DestructorCount_ = 0;
				ExecutorTaskRunnable::Waiter_ = CreateEvent(NULL, TRUE, FALSE, NULL);
			}

			virtual void TearDown() {
				::CloseHandle(ExecutorTaskRunnable::Waiter_);
				ExecutorTaskRunnable::Waiter_ = INVALID_HANDLE_VALUE;
			}
		};

		TEST_F(ExecutorTest, ExecuteBasic) 
		{
			ASSERT_TRUE(ExecutorTaskRunnable::Waiter_ != INVALID_HANDLE_VALUE); // touch static values

			const long maxCount = 100;
			for (long i = 0; i < maxCount; i++) 
			{
				IocpExecutorSingleton::Instance().Execute(
					RunnableBuilder::NewRunnable(new ExecutorTaskRunnable(maxCount))
					);
			}

			ExecutorTaskRunnable::WaitFor();

			ASSERT_EQ(maxCount, ExecutorTaskRunnable::RunningCount);
			ASSERT_EQ(maxCount, ExecutorTaskRunnable::DestructorCount_);
		}

		TEST_F(ExecutorTest, ExecuteUsingIOCPThread) 
		{
			ASSERT_TRUE(ExecutorTaskRunnable::Waiter_ != INVALID_HANDLE_VALUE); // touch static values
		
			const long maxCount = 100;
			for (long i = 0; i < maxCount; i++) 
			{
				IocpExecutorSingleton::Instance().Execute(
					RunnableBuilder::NewRunnable(new ExecutorTaskRunnable(maxCount))
					, IO_THREAD_ID_BEGIN
					);
			}

			ExecutorTaskRunnable::WaitFor();

			ASSERT_EQ(maxCount, ExecutorTaskRunnable::RunningCount);
			ASSERT_EQ(maxCount, ExecutorTaskRunnable::DestructorCount_);
		}

		TEST_F(ExecutorTest, ExecuteUsingTwoIOThreads) 
		{
			DefaultAppSettings settings;
			if(settings.NumberOfIOThreads() < 2)			
				return;

			ASSERT_TRUE(ExecutorTaskRunnable::Waiter_ != INVALID_HANDLE_VALUE); // touch static values

			const long maxCount = 100;
			for (long i = 0; i < maxCount; i++) 
			{
				ThreadIdType tid = IO_THREAD_ID_BEGIN;
				if(i % 2 == 1)
					tid = IO_THREAD_ID_BEGIN + 1;

				IocpExecutorSingleton::Instance().Execute(
					RunnableBuilder::NewRunnable(new ExecutorTaskRunnable(maxCount))
					, tid
					);
			}

			ExecutorTaskRunnable::WaitFor();

			ASSERT_EQ(maxCount, ExecutorTaskRunnable::RunningCount);
			ASSERT_EQ(maxCount, ExecutorTaskRunnable::DestructorCount_);
		}
	
	}
}