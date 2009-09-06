#include "stdafx.h"
#include "MiniDump.h"

#include "Environment.h"
#include "Path.h"
#include "File.h"

namespace Earlgrey
{
	namespace Test
	{	
		void Dump(LPEXCEPTION_POINTERS exceptionPtr)
		{
			const _txstring baseDir = Environment::BaseDirectory();
			const _txstring dumpFilePath( Path::Combine(baseDir, _T("MiniDumpTest.dmp")) );

			if( File::Exists(dumpFilePath) )
			{
				//! \todo 파일 삭제
			}

			const MINIDUMP_TYPE dumpType = MiniDumpNormal;

			MiniDump miniDump(dumpFilePath.c_str(), dumpType);
			miniDump.AddExtendedMessage(
				static_cast<MINIDUMP_STREAM_TYPE>(LastReservedStream + 1)
				, _T("사용자 정보 1")
				);

			miniDump.HandleException(exceptionPtr);	

			ASSERT_TRUE( File::Exists(dumpFilePath) );
		}


		LONG WINAPI HandleException(LPEXCEPTION_POINTERS exceptionPtr)
		{
			Dump(exceptionPtr);						
			// return EXCEPTION_CONTINUE_EXECUTION; 
			// return EXCEPTION_CONTINUE_SEARCH;
			return EXCEPTION_EXECUTE_HANDLER;
		}

		TEST(MiniDumpTest, Crash)
		{
			__try
			{
				// Access violation 
				*(int*)0 = 1; 
			}
			__except (HandleException(GetExceptionInformation()))
			{

			}

		}
	}

}