#pragma once

namespace Earlgrey
{
	template <class T> struct CreateUsingNew
	{
		static T* Create()
		{ return new T; }

		static void Destroy(T* p)
		{ delete p; }
	};
}