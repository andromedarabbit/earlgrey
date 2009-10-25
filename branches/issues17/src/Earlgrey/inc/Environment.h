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

		static _txstring BaseDirectory(); //! 응용프로그램의 바이너리 파일이 위치한 곳

		//! \todo Directory 클래스를 개발하게 되면 그 쪽에 옮기고 이곳 코드는 그 함수를 호출한다.
		static _txstring CurrentDirectory(); 
		static BOOL CurrentDirectory(const _txstring& pathName) throw(...);
		
		//! 로컬 컴퓨터의 NetBIOS 이름을 가져온다.
		static _txstring MachineName(); 

		//! 현재 사용자와 관련된 네트워크 도메인 이름을 가져온다.
		static _txstring UserDomainName(); 

		static _txstring OSVersion();

		static BOOL Is_WinXP_SP2_or_Later();
		static BOOL Is_Win_Server();

		BOOL Is64BitWindows();
	};
}
