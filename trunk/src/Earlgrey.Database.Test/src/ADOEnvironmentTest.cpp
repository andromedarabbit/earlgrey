#include "stdafx.h"
#include "ADOEnvironmentTest.h"

namespace Earlgrey {
namespace ADO {
namespace Test {

	TEST_F(ADOEnvironmentTest, GetMDACVersion)
	{
		const _txstring version = Environment::MDACVersion();
		ASSERT_FALSE(version.empty());
	}
}
}
}