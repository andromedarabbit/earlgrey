#include "stdafx.h"
#include "AppInfo.h"
#include "SingleInstance.h"
#include "StlCustom.h" 
#include "RuntimeCheck.h"

namespace Earlgrey
{

	BOOL AppInitializer::InitInstance(AppType::E_Type appType)
	{
		if( !CheckAppInstance(appType) )
			return FALSE;

		// RuntimeCheck Ȱ��ȭ
		// \todo DoRtcTermination �� ���, ���� �θ���?
		DoRtcInitialization(); 

		return TRUE;
	}


	BOOL AppInitializer::CheckAppInstance(AppType::E_Type appType)
	{
		if( gSingleInstance::Instance().IsRunning(appType) )
		{
			this->m_CurrentAppType = appType;
			return TRUE;
		}

		BOOL needMsgBox = TRUE; // TODO: �ӽ� �ڵ���

		_txstring msg = _txstring(AppType::Names[appType]) + TEXT(" has been already started!");
		if( needMsgBox )
			MessageBox(NULL, msg.c_str(), NULL, MB_OK);
		return FALSE;
	}


}
