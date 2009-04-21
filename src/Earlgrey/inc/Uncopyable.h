#pragma once

namespace Earlgrey 
{

	//! \ref Effective C++ 3th Edition Chapter 2
	class Uncopyable
	{
	protected: // �Ļ��� ��ü�� ���ؼ�
		Uncopyable() {}		// ������ �Ҹ���
		~Uncopyable() {}	// ����մϴ�.

	private:
		Uncopyable(const Uncopyable&); // ������ ����� �����մϴ�.
		const Uncopyable& operator = (const Uncopyable&);

	};

}