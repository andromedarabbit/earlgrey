#include "stdafx.h"
#include "CircularBuffer.hpp"


namespace Earlgrey
{
	namespace Test
	{
		TEST(CircularBufferTest, PushAndPop)
		{
			circular_buffer<int> cb(5);

			ASSERT_EQ(0, cb.size());
			ASSERT_EQ(5, cb.capacity());
			ASSERT_TRUE(cb.empty());
			ASSERT_TRUE(cb.max_size() > 0);

			ASSERT_TRUE(cb.push_back(1));
			ASSERT_EQ(1, cb.size());
			ASSERT_EQ(5, cb.capacity());
			ASSERT_FALSE(cb.empty());
			ASSERT_EQ(1, cb.front());
			ASSERT_EQ(1, cb.back());

			ASSERT_TRUE(cb.push_back(2));
			ASSERT_EQ(2, cb.size());
			ASSERT_EQ(5, cb.capacity());
			ASSERT_FALSE(cb.empty());
			ASSERT_EQ(1, cb.front());
			ASSERT_EQ(2, cb.back());

			
			ASSERT_TRUE(cb.push_back(3));
			ASSERT_TRUE(cb.push_back(4));
			ASSERT_TRUE(cb.push_back(5));
			ASSERT_TRUE(cb.size() == 5);
			ASSERT_TRUE(cb.capacity() == 5);
			ASSERT_TRUE(!cb.empty());
			ASSERT_TRUE(cb.front() == 1);
			ASSERT_TRUE(cb.back()  == 5);

			ASSERT_TRUE(!cb.push_back(6));
			ASSERT_TRUE(cb.size() == 5);
			ASSERT_TRUE(cb.capacity() == 5);
			ASSERT_TRUE(!cb.empty());
			ASSERT_TRUE(cb.front() == 2);
			ASSERT_TRUE(cb.back()  == 6);

			cb.pop_front();
			ASSERT_TRUE(cb.size() == 4);
			ASSERT_TRUE(cb.capacity() == 5);
			ASSERT_TRUE(!cb.empty());
			ASSERT_TRUE(cb.front() == 3);
			ASSERT_TRUE(cb.back()  == 6);

			cb.pop_front();
			ASSERT_TRUE(cb.size() == 3);
			ASSERT_TRUE(cb.front() == 4);
			ASSERT_TRUE(cb.back()  == 6);

			cb.pop_front();
			ASSERT_TRUE(cb.size() == 2);
			ASSERT_TRUE(cb.front() == 5);
			ASSERT_TRUE(cb.back()  == 6);

			cb.pop_front();
			ASSERT_TRUE(cb.size() == 1);
			ASSERT_TRUE(cb.front() == 6);
			ASSERT_TRUE(cb.back()  == 6);

			cb.pop_front();
			ASSERT_TRUE(cb.size() == 0);
			ASSERT_TRUE(cb.capacity() == 5);
			ASSERT_TRUE(cb.empty());

			// empty again

			ASSERT_TRUE(cb.push_back(7));
			ASSERT_TRUE(cb.size() == 1);
			ASSERT_TRUE(cb.capacity() == 5);
			ASSERT_TRUE(!cb.empty());
			ASSERT_TRUE(cb.front() == 7);
			ASSERT_TRUE(cb.back()  == 7);

			ASSERT_TRUE(cb.push_back(8));
			ASSERT_TRUE(cb.push_back(9));
			ASSERT_TRUE(cb.size() == 3);
			ASSERT_TRUE(!cb.empty());
			ASSERT_TRUE(cb.front() == 7);
			ASSERT_TRUE(cb.back()  == 9);

			cb.clear();
			ASSERT_TRUE(cb.size() == 0);
			ASSERT_TRUE(cb.capacity() == 5);
			ASSERT_TRUE(cb.empty());

		}


		TEST(CircularBufferTest, Indexing)
		{
			circular_buffer<int> cb(5);


			// We loop round this test several times so our data wraps around the
			// internal m_buffer array a few times.
			for (size_t n = 0; n < 10; ++n)
			{
				ASSERT_TRUE(cb.empty());

				const circular_buffer<int> &const_cb(cb);

				ASSERT_THROW(cb.at(0), std::out_of_range);
				ASSERT_THROW(cb.at(1), std::out_of_range);
				ASSERT_THROW(const_cb.at(0), std::out_of_range);
				ASSERT_THROW(const_cb.at(0), std::out_of_range);
				// we could check that operator[] explodes, but there's little point!

				ASSERT_TRUE(cb.push_back(0));
				ASSERT_TRUE(cb.push_back(1));
				ASSERT_TRUE(cb.push_back(2));
				ASSERT_TRUE(cb.at(0) == 0); ASSERT_TRUE(const_cb.at(0) == 0);
				ASSERT_TRUE(cb.at(1) == 1); ASSERT_TRUE(const_cb.at(1) == 1);
				ASSERT_TRUE(cb.at(2) == 2); ASSERT_TRUE(const_cb.at(2) == 2);
				ASSERT_TRUE(cb[0] == 0);    ASSERT_TRUE(const_cb[0] == 0);
				ASSERT_TRUE(cb[1] == 1);    ASSERT_TRUE(const_cb[1] == 1);
				ASSERT_TRUE(cb[2] == 2);    ASSERT_TRUE(const_cb[2] == 2);

				ASSERT_TRUE(cb.front() == 0);
				cb[0] = 3;
				ASSERT_TRUE(cb.front() == 3);
				ASSERT_TRUE(cb.at(0) == 3); ASSERT_TRUE(const_cb.at(0) == 3);
				ASSERT_TRUE(cb.at(1) == 1); ASSERT_TRUE(const_cb.at(1) == 1);
				ASSERT_TRUE(cb.at(2) == 2); ASSERT_TRUE(const_cb.at(2) == 2);
				ASSERT_TRUE(cb[0] == 3);    ASSERT_TRUE(const_cb[0] == 3);
				ASSERT_TRUE(cb[1] == 1);    ASSERT_TRUE(const_cb[1] == 1);
				ASSERT_TRUE(cb[2] == 2);    ASSERT_TRUE(const_cb[2] == 2);

				cb[1] = 4;
				ASSERT_TRUE(cb.at(0) == 3); ASSERT_TRUE(const_cb.at(0) == 3);
				ASSERT_TRUE(cb.at(1) == 4); ASSERT_TRUE(const_cb.at(1) == 4);
				ASSERT_TRUE(cb.at(2) == 2); ASSERT_TRUE(const_cb.at(2) == 2);
				ASSERT_TRUE(cb[0] == 3);    ASSERT_TRUE(const_cb[0] == 3);
				ASSERT_TRUE(cb[1] == 4);    ASSERT_TRUE(const_cb[1] == 4);
				ASSERT_TRUE(cb[2] == 2);    ASSERT_TRUE(const_cb[2] == 2);

				cb.pop_front();
				ASSERT_TRUE(cb[0] == 4);    ASSERT_TRUE(const_cb[0] == 4);
				ASSERT_TRUE(cb[1] == 2);    ASSERT_TRUE(const_cb[1] == 2);

				cb.pop_front();
				ASSERT_TRUE(cb[0] == 2);    ASSERT_TRUE(const_cb[0] == 2);

				cb.pop_front();
			}
		}

	}
}

