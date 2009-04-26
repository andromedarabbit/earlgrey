#pragma once

#define BOOST_ENABLE_ASSERT_HANDLER 
#include "boost/assert.hpp"


#define EARLGREY_ASSERT(expr) BOOST_ASSERT(expr)
#define EARLGREY_VERIFY(expr) BOOST_VERIFY(expr)

//! \todo 외부 라이브러리에 있는 assert를 모두 바꿔야 한다.
//! \todo boost/assert.hpp 에 정의된 assertion_failed 함수를 구현하자.