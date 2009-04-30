#include "stdafx.h"
#include "StringHelper.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(StringHelperTest, TrimStart1)
		{
			_txstring str = TEXT("		�����ٶ� ���ٻ�\r\n");
			str = StringHelper::TrimStart(str);

			_txstring expected = TEXT("�����ٶ� ���ٻ�\r\n");

			// ASSERT_TRUE(StringHelper::
			ASSERT_STREQ(expected.c_str(), str.c_str());
		}

		TEST(StringHelperTest, TrimEnd1)
		{
			_txstring str = TEXT("		�����ٶ� ���ٻ�\r\n");
			str = StringHelper::TrimEnd(str);

			_txstring expected = TEXT("		�����ٶ� ���ٻ�");

			// ASSERT_TRUE(StringHelper::
			ASSERT_STREQ(expected.c_str(), str.c_str());
		}

		TEST(StringHelperTest, Trim1)
		{
			_txstring str = TEXT("		�����ٶ� ���ٻ�\r\n");
			str = StringHelper::Trim(str);

			_txstring expected = TEXT("�����ٶ� ���ٻ�");

			// ASSERT_TRUE(StringHelper::
			ASSERT_STREQ(expected.c_str(), str.c_str());
		}

		TEST(StringHelperTest, EndsWith1) 
		{
			_txstring str = TEXT("�����ٶ󸶹ٻ�");
			_txstring suffix = TEXT("���ٻ�");

			ASSERT_TRUE(StringHelper::EndsWith(str, suffix));
		}

		TEST(StringHelperTest, EndsWith2) 
		{
			_txstring str = TEXT("�����ٶ󸶹ٻ�");
			_txstring suffix = TEXT("���ٻ�");

			ASSERT_TRUE(StringHelper::EndsWith<STRCMP_CURRENT_CULTURE>(str, suffix));
		}

		TEST(StringHelperTest, EndsWith3) 
		{
			_txstring str = TEXT("�����ٶ󸶹ٻ�");
			_txstring suffix = TEXT("����");

			ASSERT_FALSE(StringHelper::EndsWith(str, suffix));
		}

		TEST(StringHelperTest, EndsWith4) 
		{
			_txstring str = TEXT("�����ٶ󸶹ٻ�");
			_txstring suffix = TEXT("����");

			ASSERT_FALSE(StringHelper::EndsWith<STRCMP_CURRENT_CULTURE>(str, suffix));
		}

		TEST(StringHelperTest, EndsWith5) 
		{
			_txstring str = TEXT("ABCDEFG");
			_txstring suffix = TEXT("EFG");

			ASSERT_TRUE(StringHelper::EndsWith<STRCMP_CURRENT_CULTURE>(str, suffix));
		}

		TEST(StringHelperTest, EndsWith6) 
		{
			_txstring str = TEXT("ABCDEFG");
			_txstring suffix = TEXT("EF");

			ASSERT_FALSE(StringHelper::EndsWith<STRCMP_CURRENT_CULTURE_IGNORECASE>(str, suffix));
		}

		TEST(StringHelperTest, StartsWith1) 
		{
			_txstring str = TEXT("�����ٶ󸶹ٻ�");
			_txstring suffix = TEXT("������");

			ASSERT_TRUE(StringHelper::StartsWith(str, suffix));
		}


	}
}
