#include "stdafx.h"
#include "AppInitializer.h"
#include "Executor.h"
#include "Mutex.h"


namespace Earlgrey
{
	namespace Test
	{
		class ExecutorTaskRunnable : public IRunnable {

		public:
			ExecutorTaskRunnable(const long MaxRunningCount) : MaxRunningCount_(MaxRunningCount) {}
			static long RunningCount;

			virtual BOOL Init() { return TRUE;}
			virtual DWORD Run() {

				long Count = InterlockedIncrement(&RunningCount);


				if (Count == MaxRunningCount_) {
					SetEvent(Waiter_);

				}

				return 0;

			};
			virtual void Stop() {}
			virtual void Exit() {}

			static void WaitFor() {
				DWORD Success = WaitForSingleObject(Waiter_, INFINITE);// wait for 30sec
				EARLGREY_VERIFY(Success == WAIT_OBJECT_0); 
			}

		public:
			long MaxRunningCount_;
			static HANDLE Waiter_;

		};

		long ExecutorTaskRunnable::RunningCount = 0;
		HANDLE ExecutorTaskRunnable::Waiter_ = CreateEvent(NULL, TRUE, FALSE, NULL);

		TEST(ExecutorTest, ExecuteBasic) {

			AppInfo app;
			app.InitInstance(AppType::E_APPTYPE_DEFAULT);

			const long maxCount = 100;

			ASSERT_TRUE(ExecutorTaskRunnable::Waiter_ != INVALID_HANDLE_VALUE); // touch static values

			

			for (int i = 0; i < maxCount; i++) {
				IocpExecutorSingleton::Instance().Execute(RunnableBuilder::NewRunnable(new ExecutorTaskRunnable(maxCount)));
			}


			ExecutorTaskRunnable::WaitFor();
			

			ASSERT_EQ(maxCount, ExecutorTaskRunnable::RunningCount);

		}

	}
}