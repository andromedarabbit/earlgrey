#pragma once

namespace Earlgrey
{
	//! \todo ����� ����
	struct AppType
	{
		//! \note 0 �� enum ���� NONE���� ��ƾ� �Ѵ�.
		enum E_Type;

		//! \note 0 �� �̸��� TEXT("NONE")���� ��ƾ� �Ѵ�.
		const static TCHAR* Names[];
	};

	//! \note �ӽ� �ڵ�
	enum AppType::E_Type
	{
		E_APPTYPE_NONE = 0
	};
}
