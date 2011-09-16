#include "stdafx.h"
#include "Convert.h"

// #include <stdlib.h> 
#include "txstring.h"

namespace Earlgrey
{
	namespace
	{
		/*------ Base64 Encoding Table ------*/
		static const char MimeBase64[] = {
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3',
			'4', '5', '6', '7', '8', '9', '+', '/'
		};

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
	}

	
	int Convert::FromBase64(const char * const text, BYTE * bytes[], size_t& numBytes)
	{
		if(numBytes == 0)
		{
			// encode된 문자열을 decode하였을때 문자열의 길이 = ((<encode된 문자열의 길이> + 3) / 4) + 3 = ((len + 3) >> 2) + 3
			numBytes = ((strlen(text) + 3) / 4) + 3 + 1000;
			(*bytes) = new BYTE[numBytes];
			//memset(*bytes, 0, numBytes);
		}

		const char* cp;
		int space_idx = 0, phase;
		int d, prev_d = 0;
		unsigned char c;
		space_idx = 0;
		phase = 0;
		for ( cp = text; *cp != '\0'; ++cp ) {
			d = DecodeMimeBase64[(int) *cp];
			if ( d != -1 ) {
				switch ( phase ) {
	case 0:
		++phase;
		break;
	case 1:
		c = static_cast<unsigned char>( ( prev_d << 2 ) | ( ( d & 0x30 ) >> 4 ) );
		if ( space_idx < numBytes )
			(*bytes)[space_idx++] = c;
		++phase;
		break;
	case 2:
		c = static_cast<unsigned char>( ( ( prev_d & 0xf ) << 4 ) | ( ( d & 0x3c ) >> 2 ) );
		if ( space_idx < numBytes )
			(*bytes)[space_idx++] = c;
		++phase;
		break;
	case 3:
		c = static_cast<unsigned char>( ( ( prev_d & 0x03 ) << 6 ) | d );
		if ( space_idx < numBytes )
			(*bytes)[space_idx++] = c;
		phase = 0;
		break;
				}
				prev_d = d;
			}
		}
		return space_idx;
	}

	size_t Convert::ToBase64(BYTE * const bytes, size_t numBytes, char * encodedText[], size_t size)
	{
		unsigned char input[3]  = {0,0,0};
		unsigned char output[4] = {0,0,0,0};
		int   index, i, j;
		BYTE *p, *plen;
		plen           = bytes + numBytes - 1;
		if(size == 0)
		{
			size = (4 * (numBytes / 3)) + (numBytes % 3? 4 : 0) + 1;			
			// todo 메모리 할당 최적화, 스택 메모리
			*encodedText = new char[size];
		}

		j              = 0;
		for  (i = 0, p = bytes;p <= plen; i++, p++) {
			index = i % 3;
			input[index] = *p;
			if (index == 2 || p == plen) {
				output[0] = ((input[0] & 0xFC) >> 2);
				output[1] = ((input[0] & 0x3) << 4) | ((input[1] & 0xF0) >> 4);
				output[2] = ((input[1] & 0xF) << 2) | ((input[2] & 0xC0) >> 6);
				output[3] = (input[2] & 0x3F);
				(*encodedText)[j++] = MimeBase64[output[0]];
				(*encodedText)[j++] = MimeBase64[output[1]];
				(*encodedText)[j++] = index == 0? '=' : MimeBase64[output[2]];
				(*encodedText)[j++] = index <  2? '=' : MimeBase64[output[3]];
				input[0] = input[1] = input[2] = 0;
			}
		}
		(*encodedText)[j] = '\0';
		return size;
	}
}