#include "stdafx.h"
#include "MemoryManager.h"

namespace Earlgrey
{
	namespace Test
	{		
		TEST(MemoryManagerTest, Empty)
		{DWORD BlockSize	=0;
			for( DWORD i=1; i<5; i++ )
			{
				BlockSize		= (8<<((i+1)>>2)) + (2<<i);
				DBG_UNREFERENCED_LOCAL_VARIABLE(BlockSize);
				// FreeFILOArray[i]	= new FMWMRFreeStack(BlockSize);
			}
		}
	}
}