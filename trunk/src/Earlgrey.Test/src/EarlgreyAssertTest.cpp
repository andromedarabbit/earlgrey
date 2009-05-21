#include "stdafx.h"
#include "EarlgreyAssert.h"


namespace Earlgrey
{
	namespace Test
	{
		TEST(AssertTest, False2)
		{
			DWORD BlockSize = 0;

			for( DWORD i=1; i<5; i++ )
			{
				BlockSize		= (8<<((i+1)>>2)) + (2<<i);
				// FreeFILOArray[i]	= new FMWMRFreeStack(BlockSize);
				std::cout << BlockSize <<std::endl;
			}

			for( DWORD i=5; i<42; i++ )
			{
				BlockSize	=	 (4+((i+7)&3)) << (1+((i+7)>>2));
				// FreeFILOArray[i]	= new FMWMRFreeStack(BlockSize);
				std::cout << BlockSize <<std::endl;
			}



		}
		
	}
}
