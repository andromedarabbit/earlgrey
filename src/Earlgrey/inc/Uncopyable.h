#pragma once
#include "BuildConfiguration.h"

#ifdef EARLGREY_BUILD_USE_BOOST
#include <boost/utility.hpp>
#endif

namespace Earlgrey 
{

#if defined EARLGREY_BUILD_USE_BOOST
	typedef boost::noncopyable Uncopyable;
#else
	//! \ref Effective C++ 3th Edition Chapter 2
	class Uncopyable
	{
	protected: // �Ļ��� ��ü�� ���ؼ�
		Uncopyable() {}		// ������ �Ҹ���
		~Uncopyable() {}	// ����մϴ�.

	private:
		Uncopyable(const Uncopyable&); // ������ ����� �����մϴ�.
		Uncopyable& operator = (const Uncopyable&);

	};
#endif

}