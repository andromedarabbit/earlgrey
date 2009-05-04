#include "stdafx.h"
#include "UserDefinedCompilerMessage.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(CompilerMessageTest, TODO)
		{
#pragma EARLGREY_TODO(이 메시지는 무시하세요: 해야 할 일)
#pragma EARLGREY_FIXME(이 메시지는 무시하세요: 나를 고쳐줘)
		}
	}

}