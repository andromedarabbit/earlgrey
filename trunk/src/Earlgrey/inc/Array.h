#pragma once

//namespace Earlgrey { namespace DataStructure
//
//	template<typename T, size_t Size>
//	class Array : private Uncopyable
//	{
//	public:
//		explicit Array() {}
//		~Array() {}
//
//		const T& operator[](size_t index) const 
//		{
//			EARLGREY_ASSERT( index < Size );
//			return _data[index];
//		}
//		
//		T& operator[](size_t index)
//		{
//			EARLGREY_ASSERT( index < Size );
//			return _data[index];
//		}
//
//	private:
//		T _data[Size];
//	};
//
//}}