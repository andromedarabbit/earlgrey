#include "stdafx.h"
#include "NetworkStream.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(NetworkStreamTest, OutputInputStreamTest)
		{
			NetworkBuffer *buffer = new NetworkBuffer();
			NetworkOutputStream<> Output(NULL);
			Output.Initialize(buffer);
			Output << (DWORD)1 << _txstring(_T("Test")) << (__int64)87 << EndOfStream();


			NetworkInputStream<> Input;
			Input.Initialize(buffer);
			DWORD dword;
			__int64 int64;
			_txstring text;
			_txstring expectText = TEXT("Test");

			Input >> dword >> text >> int64;

			ASSERT_EQ((DWORD)1, dword);
			ASSERT_TRUE(expectText == text);
			ASSERT_EQ((__int64)87, int64);
			

		}

		TEST(NetworkStreamTest, InputStreamTest)
		{
			
		}
	}
}