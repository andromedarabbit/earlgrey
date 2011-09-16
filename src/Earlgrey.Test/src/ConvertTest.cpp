#include "stdafx.h"

#include "Convert.h"
#include "numeric_cast.hpp"

namespace Earlgrey
{
	namespace Test
	{
		TEST(ConvertTest, EncodeAndDecode)
		{
			char* plain_text = "테스트Test`1234567890-=\\[];',./";
			char* b64encoded_text = NULL;
		
			// base 64 인코딩
			// base64_encode(평문을 가리키는 포인터, 평문 길이, b64 인코딩 결과문을 가리키는 포인터)
			// 3번째 인자는 이중 포인터임에 유의해야하며, base64_encode 함수 내부에서 메모리가 할당되므로, 외부에서 꼭 해제 시켜야 한다.
			const size_t len_b64_encoded = Convert::ToBase64(
				reinterpret_cast<BYTE*>(plain_text)
				, strlen(plain_text) + 1
				, &b64encoded_text
					);
			ASSERT_LT(0, len_b64_encoded);


			// base 64 디코딩
			// 적절한 길이의 메모리 공간을 확보하고
			// 해당 공간에 디코딩된 결과를 받습니다.
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