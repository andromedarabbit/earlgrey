#include "stdafx.h"
#include "Logger.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(LoggerTest, DebugWithXString)
		{
			xwstring unicodeStr(L"가나다");
			LoggerSingleton::Instance().Debug(unicodeStr);

			xstring ansiStr("ABC");
			LoggerSingleton::Instance().Debug(ansiStr);
		}

		TEST(LoggerTest, DebugWithAutoString)
		{
			auto_wstring unicodeStr(L"가나다");
			LoggerSingleton::Instance().Debug(unicodeStr);

			auto_string ansiStr("ABC");
			LoggerSingleton::Instance().Debug(ansiStr);
		}
	}
}