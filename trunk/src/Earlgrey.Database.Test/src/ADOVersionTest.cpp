#include "stdafx.h"
#include "ADOVersion.h"

namespace Earlgrey {
namespace ADO {
namespace Test {

	TEST(ADOVersionTest, GetMDACVersion)
	{
		const _txstring version = GetMDACVersion();
		ASSERT_FALSE(version.empty());
	}
}
}
}