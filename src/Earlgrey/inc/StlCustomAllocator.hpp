#pragma once
#include "BuildConfiguration.h"

namespace Earlgrey
{
	//! \TODO ���߿� ����ȭ�� �޸� �Ҵ��ڸ� �ϼ��� �� �Ʒ� �ڵ带 ��ģ��.
	//! \note C++�� ���ø� typedef�� �������� �ʾƼ� http://www.gotw.ca/gotw/079.htm ������ ����� �����Ѵ�.
	template<typename T>
	struct StlDefaultAllocator
	{
		typedef
			EARLGREY_BUILD_STL_ALLOCATOR<T>
			// std::allocator<T> 
			// StlGreedyAllocator<T>
			Type;
	};
}