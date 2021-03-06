#include "stdafx.h"

#include "Convert.hpp"
#include "numeric_cast.hpp"
#include "StringHelper.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(ConvertTest, EncodeAndDecode1)
		{
			std::string plain_text = "테스트Test`1234567890-=\\[];',./";
			const size_t bytesLength = plain_text.length() + 1;
			
			const size_t textLength = Convert::GetMinimumLengthForEncode(bytesLength);
			char * b64encoded_text = new char[textLength];
			
			// base 64 인코딩
			const size_t len_b64_encoded = Convert::ToBase64(
				reinterpret_cast<const BYTE *>(plain_text.c_str())
				, plain_text.length() + 1
				, b64encoded_text
				, 1024
				);
			ASSERT_LT(static_cast<size_t>(0), len_b64_encoded);


			// base 64 디코딩	
			const size_t numBytes = 1024;
			BYTE * bytes = new BYTE[numBytes];
			

			const size_t len_b64 = Convert::FromBase64(b64encoded_text, textLength, bytes, numBytes);
			ASSERT_LT(static_cast<size_t>(0), numBytes);
			ASSERT_LT(static_cast<size_t>(0), len_b64);
			ASSERT_EQ(bytesLength, len_b64);

			ASSERT_STREQ(plain_text.c_str(), reinterpret_cast<char*>(bytes));

			delete b64encoded_text;
			delete bytes;
		}

		TEST(ConvertTest, EncodeEmptyBianryArray)
		{
			const size_t textLength = 1024;
			char encodedText[textLength];

			const size_t len_b64_encoded = Convert::ToBase64(
				NULL
				, 0
				, encodedText
				, textLength
				);
			ASSERT_EQ(0, len_b64_encoded);
		}

		TEST(ConvertTest, EncodeByteArray)
		{
			const size_t numBytes = 255;
			BYTE bytes[numBytes];

			for(char i = 0; i < numBytes; i++)
			{
				bytes[i] = i;
			}


			const size_t textLength = 1024; 
			char b64Text[textLength];

			const size_t len_b64_encoded = Convert::ToBase64(
				bytes
				, numBytes
				, b64Text
				, textLength
				);
			ASSERT_LT(static_cast<size_t>(0), len_b64_encoded);


			BYTE bytes2[numBytes];
			// const int len_b64 = Convert::FromBase64(b64Text, strlen(b64Text), bytes2, numBytes);
			const size_t len_b64 = Convert::FromBase64(b64Text, len_b64_encoded, bytes2, numBytes);
			ASSERT_LT(static_cast<size_t>(0), numBytes);
			ASSERT_EQ(numBytes, len_b64);

			for(char i = 0; i < numBytes; i++)
			{
				ASSERT_EQ(bytes[i], bytes2[i]);
			}

		}
	

		TEST(ConvertTest, EncodeAndDecode2)
		{
			std::string plain_text = "테스트Test`1234567890-=\\[];',./!";
			const size_t bytesLength = plain_text.length() + 1;

			const size_t textLength = 1024; 
			char b64encoded_text[textLength];

			const size_t len_b64_encoded = Convert::ToBase64(
				reinterpret_cast<const BYTE *>(plain_text.c_str())
				, bytesLength
				, b64encoded_text
				, textLength
				);
			ASSERT_LT(static_cast<size_t>(0), len_b64_encoded);


			// base 64 디코딩
			const size_t numBytes = 1024;
			BYTE bytes[numBytes];


			const size_t len_b64 = Convert::FromBase64(&b64encoded_text[0], strlen(b64encoded_text), bytes, numBytes);
			ASSERT_LT(static_cast<size_t>(0), numBytes);
			ASSERT_LT(static_cast<size_t>(0), len_b64);
			ASSERT_EQ(bytesLength, len_b64);

			ASSERT_STREQ(plain_text.c_str(), reinterpret_cast<char*>(bytes));
		}
	
		TEST(ConvertTest, DecodeEmptyString)
		{
			const char * emptyStr = "";
			
			const size_t numBytes = 1024;
			BYTE bytes[numBytes];
			memset(bytes, 1, numBytes);

			const size_t len_b64 = Convert::FromBase64(emptyStr, 0, bytes, numBytes);
			ASSERT_EQ(static_cast<size_t>(0), len_b64);

			ASSERT_EQ(1, bytes[0]);
		}

		TEST(ConvertTest, DecodeStdString)
		{
			const std::string base64Str = "QUJDREVGR0hJSktMTU5PUFFSU1RVVldYWVphYmNkZWZnaGlqa2xtbm9wcXJzdHV2d3h5ejAxMjM0NTY3ODk=";

			const size_t numBytes = 1024;
			BYTE bytes[numBytes];
			

			const size_t len_b64 = Convert::FromBase64(base64Str, bytes, numBytes);
			ASSERT_LT(static_cast<size_t>(0), len_b64);

		}

		TEST(ConvertTest, EncodeAndDecodeWString1)
		{
			const std::wstring plain_text = L"테스트Test`1234567890-=\\[];',./!";			
			const size_t bytesLength = (plain_text.length() + 1) * sizeof(WCHAR);

			const size_t textLength = 1024; 
			WCHAR b64encoded_text[textLength];

			const size_t len_b64_encoded = Convert::ToBase64(
				reinterpret_cast<const BYTE *>(plain_text.c_str())
				, bytesLength
				, b64encoded_text
				, textLength
				);
			ASSERT_LT(static_cast<size_t>(0), len_b64_encoded);


			// Decode
			const size_t numBytes = 1024;
			BYTE bytes[numBytes];


			const size_t len_b64 = Convert::FromBase64(&b64encoded_text[0], wcslen(b64encoded_text), bytes, numBytes);
			ASSERT_LT(static_cast<size_t>(0), numBytes);
			ASSERT_LT(static_cast<size_t>(0), len_b64);
			ASSERT_EQ(bytesLength, len_b64);

			ASSERT_STREQ(plain_text.c_str(), reinterpret_cast<WCHAR*>(bytes));
		}

		TEST(ConvertTest, EncodeAndDecodeWString2)
		{
			const std::wstring plain_text = L"테스트Test`1234567890-=\\[];',./!";			
			const size_t bytesLength = (plain_text.length() + 1) * sizeof(WCHAR);
			
			std::wstring b64encoded_text;
			const size_t len_b64_encoded = Convert::ToBase64(
				reinterpret_cast<const BYTE *>(plain_text.c_str())
				, bytesLength
				, b64encoded_text
				);
			ASSERT_LT(static_cast<size_t>(0), len_b64_encoded);


			// Decode
			const size_t numBytes = 1024;
			BYTE bytes[numBytes];

			const size_t len_b64 = Convert::FromBase64(b64encoded_text, bytes, numBytes);
			ASSERT_LT(static_cast<size_t>(0), numBytes);
			ASSERT_LT(static_cast<size_t>(0), len_b64);
			ASSERT_EQ(bytesLength, len_b64);

			ASSERT_STREQ(plain_text.c_str(), reinterpret_cast<WCHAR*>(bytes));
		}

		TEST(ConvertTest, EncodeWString2)
		{
			std::wstring plain_text = L"테스트Test`1234567890-=\\[];',./!";
			const size_t bytesLength = (plain_text.length() + 1) * sizeof(WCHAR);

			const size_t textLength = 1024; 
			WCHAR b64encoded_text[textLength];

			const size_t len_b64_encoded = Convert::ToBase64(
				reinterpret_cast<const BYTE *>(plain_text.c_str())
				, bytesLength
				, b64encoded_text
				, textLength
				);
			ASSERT_LT(static_cast<size_t>(0), len_b64_encoded);
		}
	}
}