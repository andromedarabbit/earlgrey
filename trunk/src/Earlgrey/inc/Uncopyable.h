#pragma once

namespace Earlgrey 
{

	//! \ref Effective C++ 3th Edition Chapter 2
	class Uncopyable
	{
	protected: // 파생된 객체에 대해서
		Uncopyable() {}		// 생성과 소멸을
		~Uncopyable() {}	// 허용합니다.

	private:
		Uncopyable(const Uncopyable&); // 하지만 복사는 방지합니다.
		const Uncopyable& operator = (const Uncopyable&);

	};

}