#pragma once
// #include "StlStackAllocator.hpp"

namespace Earlgrey
{
	//! \TODO ���߿� ����ȭ�� �޸� �Ҵ��ڸ� �ϼ��� �� �Ʒ� �ڵ带 ��ģ��.
	//! \note C++�� ���ø� typedef�� �������� �ʾƼ� http://www.gotw.ca/gotw/079.htm ������ ����� �����Ѵ�.
	template<typename T>
	struct StlDefaultAllocator
	{
		typedef 
			std::allocator<T> 
			// StlStackAllocator<T>
			Type;
	};
}