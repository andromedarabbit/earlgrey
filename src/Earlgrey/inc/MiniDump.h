#pragma once
#include "Uncopyable.h"
#include "StlCustom.h"

namespace Earlgrey
{
	////////////////////////////////////////////////////////////////////////////////
	/// \file MiniDump.h
	/// \author excel96
	/// \date 2003.11.18
	////////////////////////////////////////////////////////////////////////////////

#pragma once
	class MiniDump : private Uncopyable
	{
	private:
		

	public:
		/// \brief 미니 덤프 기능을 초기화한다.
		// void Initialize(DumpLevel dumpLevel)

	private:
		/*
		/// \brief 예외에 대한 정보를 받아서, 미니 덤프 파일을 생성한다. 		
		INT CreateMiniDump(LPEXCEPTION_POINTERS ExceptionInfo, MINIDUMP_TYPE InOption = MiniDumpNormal);
*/
		/// \brief 생성 금지
		explicit MiniDump() 
		{
		}
	};


}