#pragma once 

namespace Earlgrey
{
	class Uncopyable;

	class NoLock : private Uncopyable
	{
	public:
		inline explicit NoLock() {}
		~NoLock() {}

		inline void Lock() {}
		inline void UnLock() {}	
	};
}