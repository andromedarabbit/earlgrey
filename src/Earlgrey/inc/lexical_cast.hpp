#pragma once
#include "txsstream.h"

namespace Earlgrey
{
	namespace String
	{
		class bad_lexical_cast;

		//! simple implementation of lexical_cast
		//! \ref from Bjarne Stroustrup's lecture (http://www.stroustrup.com/Programming)
		template<typename Target, typename Source>
		Target lexical_cast(Source arg)
		{
			_txstringstream ss;
			Target result;

			if (!(ss << arg)			// read arg into stream
				|| !(ss >> result)		// read result from stream
				|| !(ss >> std::ws).eof())	// stuff left in stream?
				throw bad_lexical_cast();

			return result;
		}

		// exception used to indicate runtime lexical_cast failure
		class bad_lexical_cast : public std::bad_cast
		{
		public:
			explicit bad_lexical_cast() 
				: source(&typeid(void)), target(&typeid(void))
			{
			}

			explicit bad_lexical_cast(
			  const std::type_info &source_type_arg,
			  const std::type_info &target_type_arg
			  ) 
			  : source(&source_type_arg), target(&target_type_arg)
			{
			}

			const std::type_info &source_type() const
			{
				return *source;
			}
			const std::type_info &target_type() const
			{
				return *target;
			}

			virtual const char *what() const throw()
			{
				return "bad lexical cast: "
				  "source type value could not be interpreted as target";
			}
			virtual ~bad_lexical_cast() throw()
			{
			}

		private:
			const std::type_info *source;
			const std::type_info *target;
		};
	}
}
