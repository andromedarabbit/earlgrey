#pragma once
#include <Loki/Singleton.h>
#include <StlCustom.h>
#include "AppType.h"

namespace Earlgrey
{
	
	class AppInfo
	{
	public: // Methods
		//! �������α׷� �ν��Ͻ��� �̹� ���� ������ Ȯ���Ѵ�.
		static BOOL CheckAppInstance(AppType::E_Type appType);
		

	private:
		
	};



}
