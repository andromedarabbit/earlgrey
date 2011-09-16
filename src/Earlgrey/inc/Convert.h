#pragma once
#include "Uncopyable.h"

namespace Earlgrey
{
	class Convert : private Uncopyable
	{
	private:
		explicit Convert();

	public:
		// \ref http://www.iamcorean.net/130
		static 
			int FromBase64(const char * const text, BYTE * bytes[], size_t& numBytes);


		static
			size_t ToBase64(BYTE * const bytes, size_t numBytes, char * encodedText[], size_t size = 0);
	};
	

}