#include "stdafx.h"
#include "BasicBuffer.hpp"


namespace Earlgrey
{
	namespace Test
	{
		TEST(BasicBufferTest, SetAndGet)
		{
			basic_buffer<TCHAR> buf(5);
		
			ASSERT_EQ(5, buf.capacity());
			ASSERT_EQ(0, buf.size());
			ASSERT_TRUE(buf.empty());
			
			// set
			const TCHAR* ABCD = _T("ABCD");
			TCHAR tempStr[5];
			_tcscpy_s( tempStr, _countof(tempStr), ABCD );
			buf.set(tempStr + 0, _countof(tempStr));

			ASSERT_EQ(5, buf.size());

			// data
			const TCHAR* bufData = buf.data();
			ASSERT_STREQ(ABCD, bufData);

			// front
			const TCHAR& frontChar = buf.front();
			ASSERT_EQ(_T('A'), frontChar);

			// back
			const TCHAR& backChar = buf.back();
			ASSERT_EQ(_T('\0'), backChar);

			ASSERT_EQ(5, buf.size());
			ASSERT_FALSE(buf.empty());

			// iterator
			basic_buffer<TCHAR>::iterator it = buf.begin();
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

		TEST(BasicBufferTest, ResizeToSmallerSize)
		{
			basic_buffer<TCHAR> buf(5);

			ASSERT_EQ(5, buf.capacity());
			ASSERT_EQ(0, buf.size());
			ASSERT_TRUE(buf.empty());

			// set
			const TCHAR* ABCD = _T("ABCD");
			TCHAR tempStr[5];
			_tcscpy_s( tempStr, _countof(tempStr), ABCD );
			buf.set(tempStr + 0, _countof(tempStr));

			ASSERT_EQ(5, buf.size());

			// resize
			buf.resize(3);
			ASSERT_EQ(3, buf.size());

			// iterate
			basic_buffer<TCHAR>::iterator it = buf.begin();
			ASSERT_EQ(_T('A'), *it);

			it++;
			ASSERT_EQ(_T('B'), *it);

			it++;
			ASSERT_EQ(_T('C'), *it);

			it++;
			ASSERT_TRUE(it == buf.end());
		}

		TEST(BasicBufferTest, ResizeToLarserSize)
		{
			basic_buffer<TCHAR> buf(5);

			ASSERT_EQ(5, buf.capacity());
			ASSERT_EQ(0, buf.size());
			ASSERT_TRUE(buf.empty());

			// set
			const TCHAR* ABCD = _T("ABCD");
			TCHAR tempStr[5];
			_tcscpy_s( tempStr, _countof(tempStr), ABCD );
			buf.set(tempStr + 0, _countof(tempStr));

			ASSERT_EQ(5, buf.size());

			// resize
			buf.resize(7, _T('z'));
			ASSERT_EQ(7, buf.size());

			// iterate
			basic_buffer<TCHAR>::iterator it = buf.begin();
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
			ASSERT_EQ(_T('z'), *it);

			it++;
			ASSERT_EQ(_T('z'), *it);

			it++;
			ASSERT_TRUE(it == buf.end());

		}


		TEST(BasicBufferTest, Reserve)
		{
			basic_buffer<TCHAR> buf(5);

			ASSERT_EQ(5, buf.capacity());
			ASSERT_EQ(0, buf.size());
			ASSERT_TRUE(buf.empty());

			// set
			const TCHAR* ABCD = _T("ABCD");
			TCHAR tempStr[5];
			_tcscpy_s( tempStr, _countof(tempStr), ABCD );
			buf.set(tempStr + 0, _countof(tempStr));

			ASSERT_EQ(5, buf.size());

			ASSERT_THROW(buf.append(_T("x"), 1), std::out_of_range);

			buf.reserve(6);
			buf.append(_T("y"), 1);
			ASSERT_THROW(buf.append(_T("z"), 1), std::out_of_range);

			// iterate
			basic_buffer<TCHAR>::iterator it = buf.begin();
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
			ASSERT_EQ(_T('y'), *it);

			it++;
			ASSERT_TRUE(it == buf.end());
		}

	}
}

