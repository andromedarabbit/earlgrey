#pragma once
#include "BuildConfiguration.h"

#ifdef EARLGREY_BUILD_USE_BOOST

#pragma warning( push )
#pragma warning( disable : 4702 )
#include <boost/numeric/conversion/cast.hpp>
#pragma warning( pop )

#define EARLGREY_NUMERIC_CAST boost::numeric_cast

#else // EARLGREY_BUILD_USE_BOOST

#define EARLGREY_NUMERIC_CAST static_cast
/*
#include "EarlgreyAssert.h"
#include "if_.hpp"

#include <type_traits> 

#undef max
#undef min

namespace Earlgrey
{
	template<typename Target, typename Source>
	struct numeric_converter_for_the_same_sizes
	{
		template<typename Target, typename Source>
		struct signed_to_unsigned_converter
		{
			static Target convert(Source no)
			{
				// 같은 타입 간의 변환은 허용하지 않는다.
				EARLGREY_STATIC_ASSERT( (std::tr1::is_same<Target, Source>::value) == false); 

				// 크기가 같지만 서로 다른 두 정수 타입이라면 결국 signed/unsigned의 차이가 있다는 뜻이다.
				EARLGREY_STATIC_ASSERT(
					std::numeric_limits<Target>::is_integer == true
					&& std::numeric_limits<Source>::is_integer == true
					); 
				EARLGREY_STATIC_ASSERT(sizeof(Target) == sizeof(Source));
			
				EARLGREY_STATIC_ASSERT(std::numeric_limits<Target>::is_signed == false);
				EARLGREY_STATIC_ASSERT(std::numeric_limits<Source>::is_signed == true);

				if(no < 0)
				{
					throw std::underflow_error("bad numeric conversion: underflow");
				}

				return static_cast<Target>( no );
			}
		};

		template<typename Target, typename Source>
		struct unsigned_to_signed_converter
		{
			static Target convert(Source no)
			{
				EARLGREY_STATIC_ASSERT( (std::tr1::is_same<Target, Source>::value) == false);
				EARLGREY_STATIC_ASSERT(
					std::numeric_limits<Target>::is_integer == true
					&& std::numeric_limits<Source>::is_integer == true
					);
				EARLGREY_STATIC_ASSERT(sizeof(Target) == sizeof(Source));
				EARLGREY_STATIC_ASSERT(std::numeric_limits<Target>::is_signed == true);
				EARLGREY_STATIC_ASSERT(std::numeric_limits<Source>::is_signed == false);

				if(no > static_cast<Source>(std::numeric_limits<Target>::max()))
				{
					throw std::overflow_error("bad numeric conversion: overflow");
				}

				return static_cast<Target>( no );
			}
		};


		typedef
			typename mpl::if_<
			std::numeric_limits<Target>::is_signed == false
			, signed_to_unsigned_converter<Target, Source>
			, unsigned_to_signed_converter<Target, Source> 
			>::type 
			type;
	};

	template<typename Target, typename Source>
	struct numeric_converter_for_different_sizes
	{
		template<typename Target, typename Source>
		struct default_converter
		{
			static Target convert(Source no)
			{
				EARLGREY_STATIC_ASSERT( (std::tr1::is_same<Target, Source>::value) == false);
				EARLGREY_STATIC_ASSERT(
					std::numeric_limits<Target>::is_integer == true
					&& std::numeric_limits<Source>::is_integer == true
					);
				EARLGREY_STATIC_ASSERT(sizeof(Target) != sizeof(Source));

				if(no > std::numeric_limits<Target>::max())
				{
					throw std::overflow_error("bad numeric conversion: overflow");
				}

				if(no < std::numeric_limits<Target>::min())
				{
					throw std::underflow_error("bad numeric conversion: underflow");
				}

				return static_cast<Target>( no );
			}
		};

		typedef default_converter<Target, Source> type;
	};

	template<typename Target, typename Source>
	struct numeric_converter
	{
		typedef
			typename mpl::if_<
				sizeof(Target) == sizeof(Source)
				, typename numeric_converter_for_the_same_sizes<Target, Source>::type
				, typename numeric_converter_for_different_sizes<Target, Source>::type
			>::type 
			type;
	};

#pragma warning(push)
#pragma warning(disable:4702)
	//! \note boost의 numeric_cast와 교체하기 쉽게 이름을 똑같이 지었다.
	//! \todo boost처럼 NaN, Infinite 등을 모두 고려하면 좋겠다. 현재는 정수 변환만 고려했다.
	template<typename Target, typename Source>
	inline Target numeric_cast(Source no) throw(...)
	{
		EARLGREY_STATIC_ASSERT( (std::tr1::is_same<Target, Source>::value) == false);
		EARLGREY_STATIC_ASSERT(
			std::numeric_limits<Target>::is_integer == true
			&& std::numeric_limits<Source>::is_integer == true
			);

		typedef numeric_converter<Target, Source>::type Converter ;
		return Converter::convert(no);
	}

	template<typename T>
	inline T numeric_cast(T no) throw(...)
	{
		return no;
	}

#ifdef WIN32
	template<>
	inline unsigned long numeric_cast<unsigned long, size_t>(size_t no) throw(...)
	{
		return static_cast<unsigned long>( no );
	}
#endif

#pragma warning(pop)

} // !Earlgrey
*/
#endif // !EARLGREY_BUILD_USE_BOOST