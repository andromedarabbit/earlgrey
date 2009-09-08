#pragma once
#include "Uncopyable.h"
#include "txstring.h"

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

		static _txstring OSVersion();

		static BOOL Is_WinXP_SP2_or_Later();
		static BOOL Is_Win_Server();

		BOOL Is64BitWindows();
	};
}
