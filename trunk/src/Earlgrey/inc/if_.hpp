#pragma once

namespace Earlgrey
{
	namespace mpl
	{
		template<bool condition, class Then, class Else>
		struct if_ 
		{ 
			typedef Then type; 
		};

		template<class Then, class Else>
		// Specialized template for condition=false
		struct if_<false, Then, Else> 
		{ 
			typedef Else type; 
		};

		template <typename T>
		struct Type2Type
		{
			typedef T OriginalType;
		};
	}
}