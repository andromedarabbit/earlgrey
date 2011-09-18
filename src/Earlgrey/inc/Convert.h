#pragma once
#include "Uncopyable.h"
#include "txstring.h"

namespace Earlgrey
{
	class Convert : private Uncopyable
	{
	private:
		explicit Convert();

	public:
		// \ref http://www.iamcorean.net/130
		// Decpde
		static
			size_t GetMinimumBytesForDecode(size_t textLength);

		static 
			int FromBase64(const char * const text, size_t textLength, BYTE * bytes, size_t numBytes);
	/*	static
			int FromBase64(const xstring& text, BYTE * bytes, size_t numBytes);*/

		/*template<class _Elem, class _Traits, class _Ax>
		static 
			int FromBase64(const std::basic_string<_Elem, _Traits, _Ax> & text, BYTE * bytes, size_t numBytes)
		{
			return FromBase64(text.c_str(), text.length(), bytes, numBytes);
		}*/

		template<class _Elem, class _Traits, class _Ax>
		static 
			int FromBase64(const std::basic_string<_Elem, _Traits, _Ax> & text, BYTE * bytes, size_t numBytes);

		template<class _Ax>
		static 
			int FromBase64(const std::basic_string<char, std::char_traits<char>, _Ax> & text, BYTE * bytes, size_t numBytes)
		{
			return FromBase64(text.c_str(), text.length(), bytes, numBytes);
		}
		
		/*static 
			int FromBase64(const WCHAR * const text, size_t textLength, BYTE * bytes, size_t numBytes);
		*/	
		// Encode
		static
			size_t GetMinimumLengthForEncode(size_t numBytes);

		static
			size_t ToBase64(const BYTE * const bytes, size_t numBytes, char encodedText[], size_t textLength);
		template<class _Elem, class _Traits, class _Ax>
		static
			size_t ToBase64(const BYTE * const bytes, size_t numBytes, const std::basic_string<_Elem, _Traits, _Ax> & encodedText);

		template<class _Ax>
		static 
			size_t ToBase64(const BYTE * const bytes, size_t numBytes, const std::basic_string<char, std::char_traits<char>, _Ax> & encodedText)
		{
			const size_t textLength = GetMinimumLengthForEncode(numBytes);
			encodedText.reserve(textLength);
			return ToBase64String(bytes, numBytes, encodedText, textLength);
		}
		/*static
			size_t ToBase64(const BYTE * const bytes, size_t numBytes, xstring& encodedText);*/
		/*static
			size_t ToBase64(const BYTE * const bytes, size_t numBytes, WCHAR encodedText[], size_t textLength);*/
	};
	

}