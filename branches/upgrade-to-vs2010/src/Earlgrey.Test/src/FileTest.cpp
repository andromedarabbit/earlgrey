#include "stdafx.h"
#include "File.h"
#include "Path.h"
#include "Environment.h"
#include "EarlgreyProcess.h"

namespace Earlgrey
{
	namespace Test
	{	
		TEST(FIleTest, Exists)
		{
			const _txstring thisFilePath = Process::MainModuleFileName().c_str();

			ASSERT_TRUE2(
				File::Exists(thisFilePath)
				);
		}

		TEST(FIleTest, NotExists)
		{
			_txstring thisFilePath = Process::MainModuleFileName().c_str();
			thisFilePath += _T("NOWHERE.exe");

			ASSERT_FALSE2(
				File::Exists(thisFilePath)
				);
		}

		TEST(FileTest, DeleteFileWhichDoesNotExist)
		{
			_txstring thisFilePath = Process::MainModuleFileName().c_str();
			thisFilePath += _T("NOWHERE.exe");

			ASSERT_FALSE2(
				File::Delete(thisFilePath)
				);
		}

		TEST(FileTest, Overwrite)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			_txstring newFileName(fileName);
			newFileName = _T("Copy-") + newFileName;

			if(File::Exists(newFileName))
				ASSERT_TRUE2( File::Delete(newFileName) );

			ASSERT_TRUE2(
				File::Copy(fileName, newFileName, FALSE)
				);

			ASSERT_TRUE2(
				File::Copy(fileName, newFileName, TRUE)
				);
			
			ASSERT_TRUE2(File::Delete(newFileName));
			ASSERT_FALSE2(File::Exists(newFileName));
		}
	}
}
