#pragma once 
#include "Uncopyable.h"

namespace Earlgrey
{
	class NetworkBuffer : private Uncopyable
	{
	public:
		explicit NetworkBuffer()
		{
		}

		~NetworkBuffer()
		{
		}

		DWORD GetBufferSize()
		{
			return 0;//! todo : socket에서 사용
		}

	};
}
