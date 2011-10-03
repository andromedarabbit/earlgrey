#pragma once
#include <string>

#include "Convert.hpp"

//std::string base64_encode(BYTE const * , size_t len);
//std::string base64_decode(std::string const& s);
//
//


std::string base64_encode(BYTE const * bytes_to_encode, size_t in_len) 
{
	std::string base64Text;

	const size_t len_b64_encoded = Earlgrey::Convert::ToBase64(
		reinterpret_cast<const BYTE *>(bytes_to_encode)
		, in_len + 1
		, base64Text
		);

	EARLGREY_ASSERT(len_b64_encoded >= in_len);

	return base64Text;
}

std::string base64_decode(std::string const& encoded_string) 
{
	size_t bytesNeeded = Earlgrey::Convert::GetMinimumBytesForDecode(encoded_string.length());
	BYTE * bytes = new BYTE[bytesNeeded];

	SIZE_T len = Earlgrey::Convert::FromBase64(encoded_string, bytes, bytesNeeded);
	EARLGREY_ASSERT(len <= encoded_string.length());


	std::string decodedText(reinterpret_cast<CHAR*>(bytes));
	delete bytes;
	return decodedText;
}


