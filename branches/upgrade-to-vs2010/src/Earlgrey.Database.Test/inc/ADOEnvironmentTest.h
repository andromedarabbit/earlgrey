#pragma once
#include "ADOEnvironment.h"


namespace Earlgrey
{
	namespace ADO
	{
		namespace Test
		{
			class ADOEnvironmentTest : public ::testing::Test {
			protected:
				// Per-test-case set-up.
				// Called before the first test in this test case.
				// Can be omitted if not needed.
				static void SetUpTestCase() {
				}

				// Per-test-case tear-down.
				// Called after the last test in this test case.
				// Can be omitted if not needed.
				static void TearDownTestCase() {
				}

				// You can define per-test set-up and tear-down logic as usual.
				virtual void SetUp() {
					Environment::UninitializeADO();
				}
				virtual void TearDown() {
					Environment::UninitializeADO();
				}

				// Some expensive resource shared by all tests.
			};
		}
	}
}