#pragma once

#define EARLGREY_ENABLE_ASSERT_HANDLER 

//  boost/assert.hpp - EARLGREY_ASSERT(expr) �� ����� �����ڵ� ���ڿ��� �����ϰ� ���ƴ�.
//! \todo �ܺ� ���̺귯���� �ִ� assert�� ��� �ٲ�� �Ѵ�.
//! \todo boost/assert.hpp �� ���ǵ� assertion_failed �Լ��� ��������.

#undef EARLGREY_ASSERT

#if defined(EARLGREY_DISABLE_ASSERTS)

# define EARLGREY_ASSERT(expr) ((void)0)

#elif defined(EARLGREY_ENABLE_ASSERT_HANDLER)

namespace Earlgrey
{

	void assertion_failed(TCHAR const * expr, TCHAR const * function, TCHAR const * file, long line); // user defined

} // namespace boost

#define EARLGREY_ASSERT(expr) ((expr)? ((void)0): ::Earlgrey::assertion_failed(TEXT(#expr), TEXT(__FUNCSIG__), TEXT(__FILE__), __LINE__))

#else
# include <assert.h> // .h to support old libraries w/o <cassert> - effect is the same
# define EARLGREY_ASSERT(expr) assert(expr)
#endif

#undef EARLGREY_VERIFY

#if defined(EARLGREY_DISABLE_ASSERTS) || ( !defined(EARLGREY_ENABLE_ASSERT_HANDLER) && defined(NDEBUG) )

# define EARLGREY_VERIFY(expr) ((void)(expr))

#else

# define EARLGREY_VERIFY(expr) EARLGREY_ASSERT(expr)

#endif
