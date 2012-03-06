#pragma once
#include "Uncopyable.h"
#include "txstring.h"

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

		inline
			size_t GetMinimumLengthForEncoding(size_t numBytes)
		{
			return (4 * (numBytes / 3)) + (numBytes % 3 ? 4 : 0) + 1;
		}

		inline
			size_t GetMinimumBytesForDecoding(size_t textLength)
		{
			// return ((textLength + 3) / 4) * 3 + 1;
			return ((textLength + 3) / 4) * 3;
		}

		template<typename CharType>
		CHAR ToMultiByte(CharType c);

		template<>
		inline CHAR ToMultiByte(CHAR c)
		{
			return c;
		}

		template<>
		inline CHAR ToMultiByte(WCHAR c)
		{
			int retValue; // \note 이 값은 무슨 용도로 쓸까?
			char mbChar;
			errno_t err = wctomb_s(&retValue, &mbChar, sizeof(mbChar), c);
			if(err != 0)
			{
				throw std::exception();
			}
			return mbChar;
		}

		template<typename CharType>
		size_t FromBase64StringToBytes(const CharType * const text, size_t textLength, BYTE * bytes, size_t numBytes)
		{
			const size_t minimumBytes = GetMinimumBytesForDecoding(textLength);
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
				d = DecodeMimeBase64[(int) ToMultiByte(text[i])];
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

		template<typename CharType>
		class Base64
		{
		private:
			explicit Base64();

		public:
			static
				size_t FromString(const CharType * const text, size_t textLength, BYTE * bytes, size_t numBytes);

			template<typename TextContainerType>
			static 
				size_t ToString(const BYTE * const bytes, size_t numBytes, TextContainerType& encodedText, size_t textLength);

		};

		template<>
		class Base64<CHAR> : private Uncopyable
		{
		private:
			explicit Base64();

		public:			
			static 
				size_t FromString(const char * const text, size_t textLength, BYTE * bytes, size_t numBytes)
			{
				return FromBase64StringToBytes(text, textLength, bytes, numBytes);
			}

			// \todo 이 함수의 중복 코드도 제거 가능하겠지만 뒤로 미룬다.
			template<typename TextContainerType>
			static 
				size_t ToString(const BYTE * const bytes, size_t numBytes, TextContainerType& encodedText, size_t textLength)
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

				const size_t minimumTextLength = GetMinimumLengthForEncoding(numBytes);
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


		};

		template<>
		class Base64<WCHAR> : private Uncopyable
		{
		private:
			explicit Base64();

		public:			
			static 
				size_t FromString(const WCHAR * const text, size_t textLength, BYTE * bytes, size_t numBytes)
			{
				return FromBase64StringToBytes(text, textLength, bytes, numBytes);
			}

			template<typename TextContainerType>
			static 
				size_t ToString(const BYTE * const bytes, size_t numBytes, TextContainerType& encodedText, size_t textLength)
			{
				static const WCHAR MimeBase64[] = 
					L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
					;


				if(textLength < 1)
					throw std::exception();

				if(numBytes == 0)
				{
					encodedText[0] = L'\0'; //'\0'; 
					return 0;
				}

				const size_t minimumTextLength = GetMinimumLengthForEncoding(numBytes);
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
				(encodedText)[j] = L'\0';
				return j;
			}


		};

	}


	class Convert : private Uncopyable
	{
	private:
		explicit Convert();

	public:
		// \ref http://www.iamcorean.net/130
		// Decpde
		static
			size_t GetMinimumBytesForDecode(size_t textLength);

		template<typename CharType>
		static 
			size_t FromBase64(const CharType * const text, size_t textLength, BYTE * bytes, size_t numBytes);

		template<class _Elem, class _Traits, class _Ax>
		static 
			size_t FromBase64(const std::basic_string<_Elem, _Traits, _Ax> & text, BYTE * bytes, size_t numBytes);
		
	
		// Encode
		static
			size_t GetMinimumLengthForEncode(size_t numBytes);

		template<typename CharType>
		static 
			size_t ToBase64(const BYTE * const bytes, size_t numBytes, CharType encodedText[], size_t textLength);

		template<class _Elem, class _Traits, class _Ax>
		static 
			size_t ToBase64(const BYTE * const bytes, size_t numBytes, std::basic_string<_Elem, _Traits, _Ax> & encodedText);
	};
	
	size_t Convert::GetMinimumBytesForDecode(size_t textLength)
	{
		return GetMinimumBytesForDecoding(textLength);
	}

	template<typename CharType>
	size_t Convert::FromBase64(const CharType * const text, size_t textLength, BYTE * bytes, size_t numBytes)
	{
		return Base64<CharType>::FromString(text, textLength, bytes, numBytes);
	}


	template<class _Elem, class _Traits, class _Ax>
	size_t Convert::FromBase64(const std::basic_string<_Elem, _Traits, _Ax> & text, BYTE * bytes, size_t numBytes)
	{
		return FromBase64(text.c_str(), text.length(), bytes, numBytes);
	}

	size_t Convert::GetMinimumLengthForEncode(size_t numBytes)
	{
		return GetMinimumLengthForEncoding(numBytes);
	}

	template<typename CharType>
	size_t Convert::ToBase64(const BYTE * const bytes, size_t numBytes, CharType encodedText[], size_t textLength)
	{
		return Base64<CharType>::ToString(bytes, numBytes, encodedText, textLength);
	}

	template<class _Elem, class _Traits, class _Ax>
	size_t Convert::ToBase64(const BYTE * const bytes, size_t numBytes, std::basic_string<_Elem, _Traits, _Ax> & encodedText)
	{
		const size_t textLength = GetMinimumLengthForEncode(numBytes);
		if(encodedText.size() < textLength)
		{
			encodedText.resize(textLength);
		}
		return Base64<_Elem>::ToString(bytes, numBytes, encodedText, textLength);
	}
}