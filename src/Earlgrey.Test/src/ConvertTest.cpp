#include "stdafx.h"

#include "Convert.h"
#include "numeric_cast.hpp"

namespace Earlgrey
{
	namespace Test
	{
		TEST(ConvertTest, EncodeAndDecode)
		{
			char* plain_text = "�׽�ƮTest`1234567890-=\\[];',./";
			char* b64encoded_text = NULL;
		
			// base 64 ���ڵ�
			// base64_encode(���� ����Ű�� ������, �� ����, b64 ���ڵ� ������� ����Ű�� ������)
			// 3��° ���ڴ� ���� �������ӿ� �����ؾ��ϸ�, base64_encode �Լ� ���ο��� �޸𸮰� �Ҵ�ǹǷ�, �ܺο��� �� ���� ���Ѿ� �Ѵ�.
			const size_t len_b64_encoded = Convert::ToBase64(
				reinterpret_cast<BYTE*>(plain_text)
				, strlen(plain_text) + 1
				, &b64encoded_text
					);
			ASSERT_LT(0, len_b64_encoded);


			// base 64 ���ڵ�
			// ������ ������ �޸� ������ Ȯ���ϰ�
			// �ش� ������ ���ڵ��� ����� �޽��ϴ�.
			BYTE * bytes = NULL;
			size_t numBytes = 0;
	/*		BYTE * bytes = new BYTE[1000];
			size_t numBytes = 1000;*/
			const int len_b64 = Convert::FromBase64(b64encoded_text, &bytes, numBytes);
			ASSERT_LT(0, len_b64);

			ASSERT_STREQ(plain_text, reinterpret_cast<char*>(bytes));

			// free(dst);
			delete b64encoded_text;
		}
	}
}