#include "stdafx.h"
#include "RAII.h" // ArrayDeleter

namespace Earlgrey
{
	namespace Test
	{
		using std::tr1::shared_ptr;

		TEST(SharedPtrTest, AllocInt)
		{
			shared_ptr<int> test = shared_ptr<int> (new int(3));
		}

		TEST(SharedPtrTest, UsingArrayDeleter)
		{
			shared_ptr<double> test = shared_ptr<double> (
				new double[256]
				, ArrayDeleter<double>()
				);
		}
	}
}