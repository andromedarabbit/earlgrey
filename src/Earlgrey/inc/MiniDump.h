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
		/// \brief �̴� ���� ����� �ʱ�ȭ�Ѵ�.
		// void Initialize(DumpLevel dumpLevel)

	private:
		/*
		/// \brief ���ܿ� ���� ������ �޾Ƽ�, �̴� ���� ������ �����Ѵ�. 		
		INT CreateMiniDump(LPEXCEPTION_POINTERS ExceptionInfo, MINIDUMP_TYPE InOption = MiniDumpNormal);
*/
		/// \brief ���� ����
		explicit MiniDump() 
		{
		}
	};


}