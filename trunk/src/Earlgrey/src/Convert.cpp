#include "stdafx.h"
#include "Convert.hpp"

namespace Earlgrey
{

	size_t Convert::GetMinimumBytesForDecode(size_t textLength)
	{
		return GetMinimumBytesForDecoding(textLength);
	}

	size_t Convert::GetMinimumLengthForEncode(size_t numBytes)
	{
		return GetMinimumLengthForEncoding(numBytes);
	}

	int Convert::ToInt32(BYTE byteValue)
	{
		return byteValue & 0x000000ff;
	}
}