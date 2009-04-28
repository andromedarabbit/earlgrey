#pragma once
#include <Loki/Singleton.h>
#include <StlCustom.h>
#include "AppType.h"

namespace Earlgrey
{
	
	class AppInfo
	{
	public: // Methods
		//! 응용프로그램 인스턴스가 이미 실행 중인지 확인한다.
		static BOOL CheckAppInstance(AppType::E_Type appType);
		

	private:
		
	};



}
