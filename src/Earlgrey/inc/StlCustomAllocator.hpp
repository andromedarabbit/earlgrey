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
	//! \TODO 나중에 최적화된 메모리 할당자를 완성한 후 아래 코드를 고친다.
	//! \note C++은 템플릿 typedef를 지원하지 않아서 http://www.gotw.ca/gotw/079.htm 문서의 방법을 적용한다.
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