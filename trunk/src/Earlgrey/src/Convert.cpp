#include "stdafx.h"
#include "Convert.h"

// #include <stdlib.h> 


namespace Earlgrey
{
	namespace
	{
		/*------ Base64 Encoding Table ------*/
		/*static const char MimeBase64[] = {
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3',
			'4', '5', '6', '7', '8', '9', '+', '/'
		};*/
		/*static const char MimeBase64[] = 
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
			;*/

		
		//class MimeBase642 : private Uncopyable
		//{
		//private:
		//	explicit MimeBase642();

		//public:
		//	const char operator[] (const int index);
		//	const WCHAR operator[] (const int index);
		//};


		//const char MimeBase642::operator[] (const int index)
		//{
		//	static const char mimeBase64[] = 
		//		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		//	return mimeBase64[index];
		//}

		//const WCHAR MimeBase642::operator[] (const int index)
		//{
		//	static const WCHAR mimeBase64[] = 
		//		L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		//	return mimeBase64[index];
		//}
		

		/*------ Base64 Decoding Table ------*/
		static int DecodeMimeBase64[256] = {
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 00-0F */
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 10-1F */
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,  /* 20-2F */
			52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,  /* 30-3F */
			-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  /* 40-4F */
			15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,  /* 50-5F */
			-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  /* 60-6F */
			41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,  /* 70-7F */
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 80-8F */
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 90-9F */
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* A0-AF */
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* B0-BF */
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* C0-CF */
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* D0-DF */
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* E0-EF */
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1   /* F0-FF */
		};

		size_t GetMinimumLengthForBase64Encoding(size_t numBytes)
		{
			return (4 * (numBytes / 3)) + (numBytes % 3 ? 4 : 0) + 1;
			// return (4 * (numBytes / 3)) + (numBytes % 3 ? 4 : 0);
		}

		template<typename TextContainerType>
		size_t ToBase64String(const BYTE * const bytes, size_t numBytes, TextContainerType encodedText, size_t textLength)
		{
			static const char MimeBase64[] = 
				"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
				;

			if(textLength < 1)
				throw std::exception();

			if(numBytes == 0)
			{
				encodedText[0] = '\0'; //'\0'; 
				return 0;
			}

			const size_t minimumTextLength = GetMinimumLengthForBase64Encoding(numBytes);
			if(textLength < minimumTextLength)
			{
				throw std::exception();
			}


			unsigned char input[3]  = {0,0,0};
			unsigned char output[4] = {0,0,0,0};

			int i = 0;
			int j = 0;
			const BYTE * p = bytes;
			const BYTE * plen = bytes + numBytes - 1;

			for( ; p <= plen; i++, p++) 
			{
				int index = i % 3;
				input[index] = *p;
				if (index == 2 || p == plen) {
					output[0] = ((input[0] & 0xFC) >> 2);
					output[1] = ((input[0] & 0x3) << 4) | ((input[1] & 0xF0) >> 4);
					output[2] = ((input[1] & 0xF) << 2) | ((input[2] & 0xC0) >> 6);
					output[3] = (input[2] & 0x3F);
					(encodedText)[j++] = MimeBase64[output[0]];
					(encodedText)[j++] = MimeBase64[output[1]];
					(encodedText)[j++] = index == 0? '=' : MimeBase64[output[2]];
					(encodedText)[j++] = index <  2? '=' : MimeBase64[output[3]];
					input[0] = input[1] = input[2] = 0;
				}
			}
			(encodedText)[j] = '\0';
			return j;
		}
	}

	size_t Convert::GetMinimumBytesForDecode(size_t textLength)
	{
		/*MimeBase642 obj;
		char a = obj[1];
		DBG_UNREFERENCED_LOCAL_VARIABLE(a);*/
		// return ((textLength + 3) / 4) * 3 + 1;
		return ((textLength + 3) / 4) * 3;
	}

	size_t Convert::GetMinimumLengthForEncode(size_t numBytes)
	{
		return GetMinimumLengthForBase64Encoding(numBytes);
	}

	/*int Convert::FromBase64(const xstring& text, BYTE * bytes, size_t numBytes)
	{
		return FromBase64(text.c_str(), text.length(), bytes, numBytes);
	}
		*/

	size_t Convert::FromBase64(const char * const text, size_t textLength, BYTE * bytes, size_t numBytes)
	{
		const size_t minimumBytes = GetMinimumBytesForDecode(textLength);
		if(numBytes < minimumBytes)
		{
			throw std::exception();
		}

		size_t space_idx = 0;
		int phase = 0;
		int d = 0;
		int prev_d = 0;

		for(size_t i = 0; i < textLength; i++)
		{
			d = DecodeMimeBase64[(int) text[i]];
			if ( d != -1 ) 
			{
				switch ( phase ) 
				{
				case 0:
					++phase;
					break;
				case 1:
					if ( space_idx < numBytes )
						(bytes)[space_idx++] = static_cast<BYTE>( ( prev_d << 2 ) | ( ( d & 0x30 ) >> 4 ) );;
					++phase;
					break;
				case 2:
					if ( space_idx < numBytes )
						(bytes)[space_idx++] = static_cast<BYTE>( ( ( prev_d & 0xf ) << 4 ) | ( ( d & 0x3c ) >> 2 ) );;
					++phase;
					break;
				case 3:
					if ( space_idx < numBytes )
						(bytes)[space_idx++] = static_cast<BYTE>( ( ( prev_d & 0x03 ) << 6 ) | d );;
					phase = 0;
					break;

				}
				prev_d = d;
			}
		}
		return space_idx;
	}

	//int Convert::FromBase64(const char * const text, size_t textLength, BYTE * bytes, size_t numBytes)
	//{
	//	const size_t minimumBytes = GetMinimumBytesForDecode(textLength);
	//	if(numBytes < minimumBytes)
	//	{
	//		throw std::exception();
	//	}

	//	const char* cp = text;
	//	int space_idx = 0;
	//	int phase = 0;
	//	int d = 0;
	//	int prev_d = 0;
	//	// BYTE c;

	//	for( ; *cp != '\0'; ++cp ) 
	//	{
	//		d = DecodeMimeBase64[(int) *cp];
	//		if ( d != -1 ) 
	//		{
	//			switch ( phase ) 
	//			{
	//			case 0:
	//				++phase;
	//				break;
	//			case 1:
	//				if ( space_idx < numBytes )
	//					(bytes)[space_idx++] = static_cast<BYTE>( ( prev_d << 2 ) | ( ( d & 0x30 ) >> 4 ) );;
	//				++phase;
	//				break;
	//			case 2:
	//				if ( space_idx < numBytes )
	//					(bytes)[space_idx++] = static_cast<BYTE>( ( ( prev_d & 0xf ) << 4 ) | ( ( d & 0x3c ) >> 2 ) );;
	//				++phase;
	//				break;
	//			case 3:
	//				if ( space_idx < numBytes )
	//					(bytes)[space_idx++] = static_cast<BYTE>( ( ( prev_d & 0x03 ) << 6 ) | d );;
	//				phase = 0;
	//				break;

	//			}
	//			prev_d = d;
	//		}
	//	}
	//	return space_idx;
	//}



	/*size_t Convert::ToBase64(const BYTE * const bytes, size_t numBytes, xstring& encodedText)
	{
		size_t textLength = GetMinimumLengthForEncode(numBytes);
		encodedText.reserve(textLength);
		return ToBase64String(bytes, numBytes, encodedText, textLength);
	}*/

	

	size_t Convert::ToBase64(const BYTE * const bytes, size_t numBytes, char encodedText[], size_t textLength)
	{
		return ToBase64String(bytes, numBytes, encodedText, textLength);
	}
}