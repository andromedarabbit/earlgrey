#pragma once

// Google Test Begin
#define GTEST_HAS_TR1_TUPLE 0
#define _CRT_SECURE_DEPRECATE_MEMORY


#pragma warning( push )
#pragma warning( disable : 499 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4512 )

#if (_MSC_VER >= 1600)
#	pragma warning(disable: 4996)
#	include <gtest/gtest.h>
#else
#	include <gtest/gtest.h>
#endif

#pragma warning( pop )
// Google Test End


#define EXPECT_TRUE2(condition) \
	EXPECT_EQ(TRUE, condition)
#define EXPECT_FALSE2(condition) \
	EXPECT_EQ(FALSE, condition)
#define ASSERT_TRUE2(condition) \
	ASSERT_EQ(TRUE, condition)
#define ASSERT_FALSE2(condition) \
	ASSERT_EQ(FALSE, condition)