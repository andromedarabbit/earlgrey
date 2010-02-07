#pragma once

// External libraries
/*
EARLGREY_BUILD_USE_BOOST
*/

// Major build configuration
/*
EARLGREY_BUILD_MODE_SAFE
EARLGREY_BUILD_MODE_HIGH_PERFORMANCE
*/
//#define EARLGREY_BUILD_MODE_SAFE
#define EARLGREY_BUILD_MODE_HIGH_PERFORMANCE

// Memory allocators
#ifdef EARLGREY_BUILD_MODE_SAFE// #include <memory>
	#define EARLGREY_BUILD_STL_ALLOCATOR std::allocator
#elif defined EARLGREY_BUILD_MODE_HIGH_PERFORMANCE
	// #include "StlGreedyAllocator.hpp"
	#define EARLGREY_BUILD_STL_ALLOCATOR Earlgrey::StlGreedyAllocator
#else
#error Wrong Configuration!
#endif

namespace Earlgrey {
}