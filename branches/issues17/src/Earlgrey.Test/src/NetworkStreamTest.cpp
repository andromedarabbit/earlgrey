#include "stdafx.h"
#include "NetworkStream.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(NetworkStreamTest, OutputStreamTest)
		{
			NetworkOutputStream<> Output;
			Output << (DWORD)1 << _txstring(_T("Test")) << (__int64)87 << EndOfStream();
		}

		TEST(NetworkStreamTest, InputStreamTest)
		{
			NetworkInputStream<> Input;
			DWORD dword;
			__int64 int64;
			_txstring text;

			Input >> dword >> int64 >> text;
		}
	}
}