#pragma once

#define BOOST_ENABLE_ASSERT_HANDLER 
#include "boost/assert.hpp"


#define EARLGREY_ASSERT(expr) BOOST_ASSERT(expr)
#define EARLGREY_VERIFY(expr) BOOST_VERIFY(expr)

//! \todo �ܺ� ���̺귯���� �ִ� assert�� ��� �ٲ�� �Ѵ�.
//! \todo boost/assert.hpp �� ���ǵ� assertion_failed �Լ��� ��������.