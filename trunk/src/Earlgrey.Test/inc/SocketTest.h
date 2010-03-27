#pragma once
#include "Socket.h"

namespace Earlgrey
{
	namespace Test
	{
		class SocketTest : public ::testing::Test {
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
				Socket::UninitializeSockets();
			}
			virtual void TearDown() {
				Socket::UninitializeSockets();
			}

			// Some expensive resource shared by all tests.
		};
	}
}