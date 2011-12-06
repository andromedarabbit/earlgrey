#include "stdafx.h"
#include "StringHelper.h"

#include "numeric_cast.hpp"

namespace Earlgrey
{
	namespace String
	{
		namespace Test
		{
			TEST(StringHelperTest, TrimStart1)
			{
				_txstring str = TEXT("		가나다라 마바사\r\n");
				str = TrimStart(str);

				_txstring expected = TEXT("가나다라 마바사\r\n");

				ASSERT_STREQ(expected.c_str(), str.c_str());
			}

			TEST(StringHelperTest, TrimStart2)
			{
				_txstring trimChars = TEXT("가다");

				_txstring str = TEXT("가나다라 마바사\r\n");
				str = TrimStart(str, trimChars);

				_txstring expected = TEXT("나다라 마바사\r\n");

				ASSERT_STREQ(expected.c_str(), str.c_str());
			}

			TEST(StringHelperTest, TrimEnd1)
			{
				_txstring str = TEXT("		가나다라 마바사\r\n");
				str = TrimEnd(str);

				_txstring expected = TEXT("		가나다라 마바사");

				ASSERT_STREQ(expected.c_str(), str.c_str());
			}


			TEST(StringHelperTest, TrimEnd2)
			{
				_txstring trimChars = TEXT("\r사바\n");

				_txstring str = TEXT("		가나다라 마바사\r\n");
				str = TrimEnd(str, trimChars);
				
				_txstring expected = TEXT("		가나다라 마");

				ASSERT_STREQ(expected.c_str(), str.c_str());
			}

			TEST(StringHelperTest, Trim1)
			{
				_txstring str = TEXT("		가나다라 마바사\r\n");
				str = Trim(str);

				_txstring expected = TEXT("가나다라 마바사");

				ASSERT_STREQ(expected.c_str(), str.c_str());
			}

			TEST(StringHelperTest, Trim2)
			{
				_txstring trimChars = TEXT("사바나가");

				_txstring str = TEXT("가나다라 마바사");
				str = Trim(str, trimChars);

				_txstring expected = TEXT("다라 마");

				ASSERT_STREQ(expected.c_str(), str.c_str());
			}

			TEST(StringHelperTest, TrimEmptyString)
			{
				_txstring str = TEXT("");
				str = Trim(str);

				_txstring expected = TEXT("");

				ASSERT_STREQ(expected.c_str(), str.c_str());
			}

			TEST(StringHelperTest, ToLowerInPlace1)
			{
				_txstring str = TEXT("AbCd사바나가EfGh");

				ToLower(&str);

				_txstring expected = TEXT("abcd사바나가efgh");

				ASSERT_STREQ(expected.c_str(), str.c_str());
			}

			TEST(StringHelperTest, ToLowerWithCopy1)
			{
				_txstring str = TEXT("AbCd사바나가EfGh");

				_txstring newStr = ToLower(str);

				_txstring expected = TEXT("abcd사바나가efgh");

				ASSERT_STREQ(expected.c_str(), newStr.c_str());
			}

			TEST(StringHelperTest, ToUpperInPlace1)
			{
				_txstring str = TEXT("AbCd사바나가EfGh");

				ToUpper(&str);

				_txstring expected = TEXT("ABCD사바나가EFGH");

				ASSERT_STREQ(expected.c_str(), str.c_str());
			}

			TEST(StringHelperTest, ToUpperWithCopy1)
			{
				_txstring str = TEXT("AbCd사바나가EfGh");

				_txstring newStr = ToUpper(str);

				_txstring expected = TEXT("ABCD사바나가EFGH");

				ASSERT_STREQ(expected.c_str(), newStr.c_str());
			}

			TEST(StringHelperTest, EndsWith1) 
			{
				_txstring str = TEXT("가나다라마바사");
				_txstring suffix = TEXT("마바사");

				ASSERT_TRUE2(EndsWith(str, suffix));
			}

			TEST(StringHelperTest, EndsWith2) 
			{
				_txstring str = TEXT("가나다라마바사");
				_txstring suffix = TEXT("마바사");

				ASSERT_TRUE2(EndsWith<STRCMP_CURRENT_CULTURE>(str, suffix));
			}

			TEST(StringHelperTest, EndsWith3) 
			{
				_txstring str = TEXT("가나다라마바사");
				_txstring suffix = TEXT("마바");

				ASSERT_FALSE2(EndsWith(str, suffix));
			}

			TEST(StringHelperTest, EndsWith4) 
			{
				_txstring str = TEXT("가나다라마바사");
				_txstring suffix = TEXT("마바");

				ASSERT_FALSE2(EndsWith<STRCMP_CURRENT_CULTURE>(str, suffix));
			}

			TEST(StringHelperTest, EndsWith5) 
			{
				_txstring str = TEXT("ABCDEFG");
				_txstring suffix = TEXT("EFG");

				ASSERT_TRUE2(EndsWith<STRCMP_CURRENT_CULTURE>(str, suffix));
			}

			TEST(StringHelperTest, EndsWith6) 
			{
				_txstring str = TEXT("ABCDEFG");
				_txstring suffix = TEXT("EF");

				ASSERT_FALSE2(EndsWith<STRCMP_CURRENT_CULTURE_IGNORECASE>(str, suffix));
			}

			TEST(StringHelperTest, StartsWith1) 
			{
				const _txstring str = TEXT("가나다라마바사");
				const _txstring prefix = TEXT("가나다");

				ASSERT_TRUE2(StartsWith(str, prefix));
			}

			TEST(StringHelperTest, StartsWith2) 
			{
				const _txstring str = TEXT("1.0.0.930");
				const _txstring prefix = TEXT("1.0.0.");

				ASSERT_TRUE2(StartsWith(str, prefix));
			}

			TEST(StringHelperTest, IsNullOrEmpty)
			{
				TCHAR * str = _T("");
				ASSERT_TRUE2(IsNullOrEmpty(str));
				ASSERT_TRUE2(IsNullOrEmpty(NULL));

				str = _T(" ");
				ASSERT_FALSE2(IsNullOrEmpty(str));
			}



			// Encoding 
			TEST(StringHelperTest, ToUnicode)
			{
				const std::string src1("가나 다라 마 바 사 ");
				const std::wstring expected1(_T("가나 다라 마 바 사 "));

				const WCHAR * dst1 = String::ToUnicode(src1.c_str(), EARLGREY_NUMERIC_CAST<int>(src1.length()), CP_THREAD_ACP);
				
				ASSERT_TRUE(dst1 != NULL);
				ASSERT_EQ(expected1, dst1);

				const std::string src2("ASDJL MLKMQWLEIOJFAS");
				const std::wstring expected2(_T("ASDJL MLKMQWLEIOJFAS"));

				const WCHAR * dst2 = String::ToUnicode(src2.c_str(), EARLGREY_NUMERIC_CAST<int>(src2.length()), CP_THREAD_ACP);

				ASSERT_TRUE(dst2 != NULL);
				ASSERT_EQ(expected2, dst2);
			}

			TEST(StringHelperTest, FromStringToUnicode)
			{
				const std::string src1("가나 다라 마 바 사 ");
				const std::wstring expected1(_T("가나 다라 마 바 사 "));

				const WCHAR * dst1 = String::ToUnicode(src1);

				ASSERT_TRUE(dst1 != NULL);
				ASSERT_EQ(expected1, dst1);

				const std::string src2("ASDJL MLKMQWLEIOJFAS");
				const std::wstring expected2(_T("ASDJL MLKMQWLEIOJFAS"));

				const WCHAR * dst2 = String::ToUnicode(src2);

				ASSERT_TRUE(dst2 != NULL);
				ASSERT_EQ(expected2, dst2);
			}

			TEST(StringHelperTest, EmptyStringToUnicode)
			{
				const std::string src;
				const std::wstring expected;

				const WCHAR * dst = String::ToUnicode(src.c_str(), EARLGREY_NUMERIC_CAST<int>(src.length()), CP_THREAD_ACP);

				ASSERT_TRUE(dst != NULL);
				ASSERT_EQ(expected, dst);
			}

			TEST(StringHelperTest, FromUnicode)
			{
				const std::wstring src1(_T("가나 다라 마 바 사 "));
				const std::string expected1("가나 다라 마 바 사 ");

				const CHAR * dst1 = String::FromUnicode(
					src1.c_str()
					, EARLGREY_NUMERIC_CAST<int>(src1.length() * sizeof(WCHAR))
					, CP_THREAD_ACP
					);

				ASSERT_TRUE(dst1 != NULL);
				ASSERT_EQ(expected1, dst1);

				const std::wstring src2(_T("ASDJL MLKMQWLEIOJFAS"));
				const std::string expected2("ASDJL MLKMQWLEIOJFAS");

				const CHAR * dst2 = String::FromUnicode(
					src2.c_str()
					, EARLGREY_NUMERIC_CAST<int>(src2.length() * sizeof(WCHAR) )
					, CP_THREAD_ACP
					);

				ASSERT_TRUE(dst2 != NULL);
				ASSERT_EQ(expected2, dst2);
			}

			TEST(StringHelperTest, FromUnicode2)
			{
				// 동일한 메모리를 재활용하기 때문에 반환 값을 복사하지 않으면 아래와 같이 잘못된 값이 나오기도 한다.
				std::wstring wstr1 = L"얼그레이";
				std::wstring wstr2 = L"plaintext";

				const char * str1 = String::FromUnicode(wstr1);
				const char * str2 = String::FromUnicode(wstr2);

				ASSERT_STREQ("plaintext", str1);
				ASSERT_STREQ("plaintext", str2);
			}

			TEST(StringHelperTest, FromWStringToAnsi)
			{
				const std::wstring src1(_T("가나 다라 마 바 사 "));
				const std::string expected1("가나 다라 마 바 사 ");

				const CHAR * dst1 = String::FromUnicode(src1);

				ASSERT_TRUE(dst1 != NULL);
				ASSERT_EQ(expected1, dst1);

				const std::wstring src2(_T("ASDJL MLKMQWLEIOJFAS"));
				const std::string expected2("ASDJL MLKMQWLEIOJFAS");

				const CHAR * dst2 = String::FromUnicode(src2);

				ASSERT_TRUE(dst2 != NULL);
				ASSERT_EQ(expected2, dst2);
			}

			TEST(StringHelperTest, FromUnicodeWithEmptyString)
			{
				const std::wstring src;
				const std::string expected;

				const CHAR * dst = String::FromUnicode(
					src.c_str()
					, EARLGREY_NUMERIC_CAST<int>(src.length() * sizeof(WCHAR))
					, CP_THREAD_ACP
					);

				ASSERT_TRUE(dst != NULL);
				ASSERT_EQ(expected, dst);
			}
		}
	}
}
