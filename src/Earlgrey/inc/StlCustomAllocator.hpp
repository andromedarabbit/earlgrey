#pragma once
#include "BuildConfiguration.h"

#ifdef EARLGREY_BUILD_MODE_SAFE
#include <memory>
#elif defined EARLGREY_BUILD_MODE_HIGH_PERFORMANCE
#include "StlGreedyAllocator.hpp"
#else
#error Wrong Configuration!
#endif

namespace Earlgrey
{
	//! \TODO ���߿� ����ȭ�� �޸� �Ҵ��ڸ� �ϼ��� �� �Ʒ� �ڵ带 ��ģ��.
	//! \note C++�� ���ø� typedef�� �������� �ʾƼ� http://www.gotw.ca/gotw/079.htm ������ ����� �����Ѵ�.
	template<typename T>
	struct StlDefaultAllocator
	{
		typedef
			EARLGREY_BUILD_STL_ALLOCATOR<T>
			// std::allocator<T> 
			// StlGreedyAllocator<T>
			Type;
	};
}