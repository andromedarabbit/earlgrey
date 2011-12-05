#pragma once
#include <string>

#include "Convert.hpp"


namespace Earlgrey
{
	namespace Extension
	{
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


		template<class _Elem, class _Traits, class _Ax>
		static 
			std::basic_string<_Elem, _Traits, _Ax> base64_decode(std::basic_string<_Elem, _Traits, _Ax> const& encoded_string) 
		{
			size_t bytesNeeded = Earlgrey::Convert::GetMinimumBytesForDecode(encoded_string.length());
			BYTE * bytes = new BYTE[bytesNeeded];
			// BYTE * bytes = Malloca(bytesNeeded);
			// std::tr1::shared_ptr<BYTE> bytes( new BYTE[bytesNeeded] );
			

			SIZE_T len = Earlgrey::Convert::FromBase64(encoded_string, bytes, bytesNeeded);
			EARLGREY_ASSERT(len <= encoded_string.length());


			std::basic_string<_Elem, _Traits, _Ax> decodedText(reinterpret_cast<_Elem*>(bytes));
			delete bytes;
			return decodedText;
		}

	} // end of namespace Extension
} // end of namespace Earlgrey