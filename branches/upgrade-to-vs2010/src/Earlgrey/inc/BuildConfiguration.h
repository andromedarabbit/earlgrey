#pragma once

// External libraries 
// EARLGREY_BUILD_USE_BOOST 
#ifndef EARLGREY_BUILD_USE_BOOST
// #define EARLGREY_BUILD_USE_BOOST
#endif 

// Do not remove or uncomment the following lines, which are for build scripts!
/* {BuildTool}
#ifndef EARLGREY_BUILD_USE_BOOST
#define EARLGREY_BUILD_USE_BOOST
#endif 
{BuildTool} */

// Major build configuration
// EARLGREY_BUILD_MODE_SAFE
// EARLGREY_BUILD_MODE_HIGH_PERFORMANCE
#ifndef EARLGREY_BUILD_MODE_HIGH_PERFORMANCE
#define EARLGREY_BUILD_MODE_HIGH_PERFORMANCE
#ifdef EARLGREY_BUILD_MODE_SAFE
#error Wrong Configuration!
#endif
#endif

// Memory allocators
#ifdef EARLGREY_BUILD_MODE_SAFE // #include <memory>
	#define EARLGREY_BUILD_STL_ALLOCATOR std::allocator
#elif defined EARLGREY_BUILD_MODE_HIGH_PERFORMANCE
	// #include "StlGreedyAllocator.hpp"
	#define EARLGREY_BUILD_STL_ALLOCATOR Earlgrey::StlGreedyAllocator
#else
#error Wrong Configuration!
#endif

namespace Earlgrey {
}