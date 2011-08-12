#include "stdafx.h"
#include "MiniDump.h"

#include "StackWriter.h"

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

			if( File::Exists(dumpFilePath.c_str()) )
			{
				ASSERT_TRUE(File::Delete(dumpFilePath));
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

		void WriteSummary(LPEXCEPTION_POINTERS exceptionPtr)
		{
			const _txstring baseDir = Environment::BaseDirectory();
			const _txstring dumpFilePath( Path::Combine(baseDir, _T("MiniDumpTest.txt")) );

			if( File::Exists(dumpFilePath.c_str()) )
			{
				// 파일 삭제 안해주면 테스트 성공/실패 반복하므로 여기에서 삭제함.
				File::Delete( dumpFilePath );
			}

			StackWriter sw(dumpFilePath.c_str(), StackWalker::OptionsAll);
			sw.HandleException(exceptionPtr);	

			ASSERT_TRUE( File::Exists(dumpFilePath) );
		}


		LONG WINAPI HandleException(LPEXCEPTION_POINTERS exceptionPtr)
		{
			Dump(exceptionPtr);						
			WriteSummary(exceptionPtr);
			
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