// ref: http://blogs.msdn.com/abhinaba/archive/2008/10/27/c-c-compile-time-asserts.aspx
// ref: BOOST Library
#pragma once

#define EARLGREY_JOIN( X, Y ) EARLGREY_JOIN2(X,Y)
#define EARLGREY_JOIN2( X, Y ) X##Y

namespace Earlgrey
{
	template <bool> struct STATIC_ASSERT_FAILURE;
	template <> struct STATIC_ASSERT_FAILURE<true> { enum { value = 1 }; };

	template<int x> struct static_assert_test{};
}

// 참고: VS2008 에서는 컴파일 에러 보여줄 때 아래 재정의 타입 이름을 안보여줌 (_static_assert_typedefN)
#define EARLGREY_STATIC_ASSERT(x) \
	typedef ::Earlgrey::static_assert_test<\
	sizeof(::Earlgrey::STATIC_ASSERT_FAILURE< (bool)( x ) >)>\
	EARLGREY_JOIN(_static_assert_typedef, __LINE__)


#define VERIFY_EXPLICIT_CAST(from, to) EARLGREY_STATIC_ASSERT(sizeof(from) == sizeof(to)) 

