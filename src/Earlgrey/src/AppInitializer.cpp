#include "stdafx.h"
#include "AppInitializer.h"
#include "SingleInstance.h"
#include "StlCustom.h" 
#include "RuntimeCheck.h"
#include "ProcessInitializer.h"

namespace Earlgrey
{

	BOOL AppInfo::InitInstance(AppType::E_Type appType)
	{
		if( !CheckAppInstance(appType) )
			return FALSE;

		// RuntimeCheck Ȱ��ȭ
		// \todo DoRtcTermination �� ���, ���� �θ���?
		DoRtcInitialization(); 

		// ���μ��� �켱����
		ProcessInitializer processInitializer;
		if( !processInitializer.Run() )
			return FALSE;

		return TRUE;
	}


	BOOL AppInfo::CheckAppInstance(AppType::E_Type appType)
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
