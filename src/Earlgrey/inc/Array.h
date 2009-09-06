#pragma once

namespace Earlgrey {

	//! \note std::tr1 에 동일한 클래스 array가 있습니다. 이 코드는 필요 없겠네요.
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