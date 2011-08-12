#include "stdafx.h"
#include "ProcessStartInfo.h"

namespace Earlgrey
{
	ProcessStartInfo::ProcessStartInfo()
		: m_fileName()
		, m_arguments()
	{
	}

	ProcessStartInfo::ProcessStartInfo(const _txstring& fileName)
		: m_fileName(fileName)
		, m_arguments()
	{
	}

	ProcessStartInfo::ProcessStartInfo(const _txstring& fileName, const _txstring& arguments)
		: m_fileName(fileName)
		, m_arguments(arguments)
	{
	}
}