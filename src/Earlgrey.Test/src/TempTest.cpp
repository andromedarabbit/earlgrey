#include "stdafx.h"
// #include "BasicBuffer.hpp"

#include "xvector.h"
#include "xlist.h"

namespace Earlgrey
{
	namespace Test
	{
		/*
		TEST(WinAPI, IsWow64Process)
		{
			BOOL b = FALSE;
			IsWow64Process(GetCurrentProcess(), &b);
			ASSERT_TRUE2(b);
		}
		*/

		TEST(BackInserterTest, Vector)
		{
			typedef xvector<int>::Type Container;

			Container container;
			container.push_back(1);
			container.push_back(2);
			container.push_back(3);

			ASSERT_EQ(static_cast<size_t>(3), container.size());

			std::back_inserter(container) = 4;
			ASSERT_EQ(static_cast<size_t>(4), container.size());
			ASSERT_EQ(4, container[3]);
		}

		TEST(BackInserterTest, List)
		{
			typedef xlist<int>::Type Container;

			Container container;
			container.push_back(1);
			container.push_back(2);
			container.push_back(3);

			ASSERT_EQ(static_cast<size_t>(3), container.size());

			std::back_inserter(container) = 4;
			ASSERT_EQ(static_cast<size_t>(4), container.size());
			ASSERT_EQ(4, container.back());
		}
	}
}

