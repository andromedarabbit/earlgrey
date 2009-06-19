#pragma once

namespace Earlgrey {

	template<typename T, size_t Size>
	class Array : private Uncopyable
	{
	public:
		explicit Array() {}
		~Array() {}

		static T& OutOfRange()
		{
			static T outOfRange;
			return outOfRange;
		}

		const T& operator[](size_t index) const 
		{
			//EARLGREY_ASSERT( index < Size );
			if (index >= Size)
			{
				return OutOfRange();
			}
			return _array[index];
		}
		
		T& operator[](size_t index)
		{
			return const_cast<T&>( static_cast<const Array&>( (*this) )[index] );
		}

	private:
		T _array[Size];
	};

}