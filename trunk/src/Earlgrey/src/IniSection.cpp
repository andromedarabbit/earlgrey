#include "stdafx.h"
#include "IniSection.h"

#include "numeric_cast.hpp"

namespace Earlgrey 
{
	_txstring IniSection::ReadString(const _txstring& keyName, const _txstring& defaultValue) const
	{
		EARLGREY_ASSERT(keyName.empty() == false);

		m_Buffer.clear();

		DWORD length = 0;
		for(;;)
		{
			LPTSTR rawBuffer = m_Buffer.data();
			const DWORD size = EARLGREY_NUMERIC_CAST<DWORD>(m_Buffer.capacity());

			length = GetPrivateProfileString(m_Name.c_str(), keyName.c_str(), defaultValue.c_str(), rawBuffer, size, m_FilePath.c_str());
			if(length != size - 2)
				break;

			m_Buffer.reserve(size * 2);
		}

		return _txstring(m_Buffer.data());
	}



}