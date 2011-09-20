/* 
   base64.cpp and base64.h

   Copyright (C) 2004-2008 Ren?Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   Ren?Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/


// Fully reimplemeted by Jay
#include "stdafx.h"
#include "base64.h"
#include "Convert.hpp"

std::string base64_encode(unsigned char const* bytes_to_encode, size_t in_len) 
{
 	std::string base64Text;

	const size_t len_b64_encoded = Earlgrey::Convert::ToBase64(
		reinterpret_cast<const BYTE *>(bytes_to_encode)
		, in_len + 1
		, base64Text
		);

	assert(len_b64_encoded >= in_len);

	return base64Text;
}

std::string base64_decode(std::string const& encoded_string) 
{
	size_t bytesNeeded = Earlgrey::Convert::GetMinimumBytesForDecode(encoded_string.length());
	BYTE * bytes = new BYTE[bytesNeeded];

	SIZE_T len = Earlgrey::Convert::FromBase64(encoded_string, bytes, bytesNeeded);
	assert(len <= encoded_string.length());


	std::string decodedText(reinterpret_cast<CHAR*>(bytes));
	delete bytes;
	return decodedText;
}
