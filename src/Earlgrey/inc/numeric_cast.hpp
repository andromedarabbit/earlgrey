#pragma once
#include "EarlgreyAssert.h"
#include "if_.hpp"

namespace Earlgrey
{
	namespace Math
	{
		template<typename Target, typename Source>
		struct numeric_converter_for_the_same_sizes
		{
			template<typename Target, typename Source>
			struct unsigned_to_signed_converter
			{
				static Target convert(Source no)
				{
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
			struct signed_to_unsigned_converter
			{
				static Target convert(Source no)
				{
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
				std::numeric_limits<Target>::is_signed == true
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

		//! \note boost의 numeric_cast와 교체하기 쉽게 이름을 똑같이 지었다.
		//! \todo boost처럼 NaN, Infinite 등을 모두 고려하면 좋겠다. 현재는 정수 변환만 고려했다.
		template<typename Target, typename Source>
		inline Target numeric_cast(Source no) throw(...)
		{
			EARLGREY_STATIC_ASSERT(
				std::numeric_limits<Target>::is_integer == true
				&& std::numeric_limits<Source>::is_integer == true
				);

			typedef numeric_converter<Target, Source>::type Converter ;
			return Converter::convert(no);
		}


	}
}
