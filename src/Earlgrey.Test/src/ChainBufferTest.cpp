#include "stdafx.h"
#include "ChainBuffer.hpp"


namespace Earlgrey
{
	namespace Test
	{
		TEST(ChainBufferTest, SetAndGet)
		{
			chain_buffer<TCHAR> buf(5);

			ASSERT_EQ(5, buf.capacity());
			ASSERT_EQ(0, buf.size());
			ASSERT_TRUE(buf.empty());

			// set
			const TCHAR* ABCD = _T("ABCD");
			TCHAR tempStr[5];
			_tcscpy_s( tempStr, _countof(tempStr), ABCD );
			buf.set(tempStr + 0, _countof(tempStr));
			
			//// front
			const TCHAR& frontChar = buf.front();
			ASSERT_EQ(_T('A'), frontChar);

			//// back
			const TCHAR& backChar = buf.back();
			ASSERT_EQ(_T('\0'), backChar);

			ASSERT_EQ(_countof(tempStr), buf.size());
			ASSERT_FALSE(buf.empty());

			//// iterator
			chain_buffer<TCHAR>::iterator it = buf.begin();
			ASSERT_EQ(_T('A'), *it);

			it++;
			ASSERT_EQ(_T('B'), *it);

			it++;
			ASSERT_EQ(_T('C'), *it);

			it++;
			ASSERT_EQ(_T('D'), *it);

			it++;
			ASSERT_EQ(_T('\0'), *it);

			it++;
			ASSERT_TRUE(it == buf.end());

		}

		TEST(ChainBufferTest, AutoResize)
		{
			const int COUNT = 2;

			chain_buffer<TCHAR> buf(5);

			ASSERT_EQ(5, buf.capacity());
			ASSERT_EQ(0, buf.size());
			ASSERT_TRUE(buf.empty());

			// set
			const TCHAR* ABCD = _T("ABCD");
			TCHAR tempStr[5];
			_tcscpy_s( tempStr, _countof(tempStr), ABCD );

			for(int i = 0; i < COUNT; i++)
			{
				buf.set(tempStr + 0, _countof(tempStr));
			}

			//// front
			const TCHAR& frontChar = buf.front();
			ASSERT_EQ(_T('A'), frontChar);

			//// back
			const TCHAR& backChar = buf.back();
			ASSERT_EQ(_T('\0'), backChar);

			ASSERT_EQ(_countof(tempStr) * COUNT, buf.size());
			ASSERT_FALSE(buf.empty());

			//// iterator
			chain_buffer<TCHAR>::iterator it = buf.begin();
			for(int i = 0; i < COUNT; i++)
			{
				ASSERT_EQ(_T('A'), *it);

				it++;
				ASSERT_EQ(_T('B'), *it);

				it++;
				ASSERT_EQ(_T('C'), *it);

				it++;
				ASSERT_EQ(_T('D'), *it);

				it++;
				ASSERT_EQ(_T('\0'), *it);

				it++;
			}
			
			ASSERT_TRUE(it == buf.end());

		}

		TEST(ChainBufferTest, BasicBufferUse)
		{
			chain_buffer<basic_buffer<BYTE>> buf(1024);

			basic_buffer<BYTE> bbuf(1);
			BYTE tempStr = 'a';
			bbuf.set(&tempStr , 1);

			buf.set(&bbuf, bbuf.size());
		}
	}
}

