#pragma once

namespace Earlgrey
{
	//! \todo 사용자 정의
	struct AppType
	{
		//! \note 0 번 enum 값은 NONE으로 잡아야 한다.
		enum E_Type;

		//! \note 0 번 이름은 TEXT("NONE")으로 잡아야 한다.
		const static TCHAR* Names[];
	};

	//! \note 임시 코드
	enum AppType::E_Type
	{
		E_APPTYPE_NONE = 0
	};
}
