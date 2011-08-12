#include "stdafx.h"
#include "IniSection.h"

#include "numeric_cast.hpp"

namespace Earlgrey 
{
	_txstring IniSection::ReadString(const _txstring& keyName) const
	{
		EARLGREY_ASSERT(keyName.empty() == false);

		m_Buffer.clear();

		DWORD length = 0;
		for(;;)
		{
			LPTSTR rawBuffer = m_Buffer.data();
			const DWORD size = EARLGREY_NUMERIC_CAST<DWORD>(m_Buffer.capacity());

			length = GetPrivateProfileString(m_Name.c_str(), keyName.c_str(), _T(";"), rawBuffer, size, m_FilePath.c_str());
			if(length != size - 1)  // 왜 -1 인지는 MSDN 문서를 확인할 것!
				break;

			m_Buffer.reserve(size * 2); 
		}

		_txstring valueFound(m_Buffer.data());
		if(valueFound == _T(";"))
			throw std::exception();
		return valueFound;
	}
}
