#pragma once
#include "Uncopyable.h"
#include "StlCustom.h"

namespace Earlgrey 
{ 
	class Environment : private Uncopyable
	{
	private:
		explicit Environment();

	public:
		static const TCHAR* const NewLine();

		static DWORD ProcessorCount();

		static _txstring BaseDirectory(); //! �������α׷��� ���̳ʸ� ������ ��ġ�� ��

		//! \todo Directory Ŭ������ �����ϰ� �Ǹ� �� �ʿ� �ű�� �̰� �ڵ�� �� �Լ��� ȣ���Ѵ�.
		static _txstring CurrentDirectory(); 
		static BOOL CurrentDirectory(const _txstring& pathName) throw(...);
		
		//! ���� ��ǻ���� NetBIOS �̸��� �����´�.
		static _txstring MachineName(); 

		//! ���� ����ڿ� ���õ� ��Ʈ��ũ ������ �̸��� �����´�.
		static _txstring UserDomainName(); 

		// \todo ������ ��?
		// Version

	};
}
