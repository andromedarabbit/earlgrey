#include "stdafx.h"

namespace Earlgrey
{
	namespace Test
	{	
		TEST(ProcessTest, GetConsoleProcessList)
		{
			const DWORD MAX_COUNT = 256;
			DWORD pidList[MAX_COUNT];
			::ZeroMemory(pidList, sizeof(pidList));

			DWORD processCount = ::GetConsoleProcessList(pidList, MAX_COUNT);
			ASSERT_GT(processCount, static_cast<DWORD>(0));


			DWORD thisPID = ::GetCurrentProcessId();
			
			BOOL thisProcessIsConsole = FALSE;
			for(DWORD i = 0; i < processCount; i++)
			{
				DWORD pid = pidList[i];
				if(pid == thisPID)
				{
					thisProcessIsConsole = TRUE;
				}
			}
			ASSERT_TRUE(thisProcessIsConsole);
		}
	}
}