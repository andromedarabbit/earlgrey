#pragma once
#include "Uncopyable.h"
#include "StlCustom.h"

namespace Earlgrey
{
	class SystemInfo : private Uncopyable
	{
	// private:
		// explicit SystemInfo(); // 구현하지 않는다.

	private:
		SYSTEM_INFO m_SystemInfo;

	public:
		explicit SystemInfo()
		{
			GetSystemInfo(&m_SystemInfo);
		}

		// TODO: 아직 Assert가 없어서 대충 짜본다.
		/*
		void ValidateSystem() const
		{			
			WORD processorArchitecture = m_SystemInfo.wProcessorArchitecture;

			BYTE processorRivision = HIBYTE(m_SystemInfo.wProcessorRevision );
			if(processorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				return;

			if(processorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
			{
				// 일단 Core Duo 2 이상만 정상처리하는 편이 좋지 않을까?
				return;
			}

			// crash
		}
		*/

		DWORD NumberOfProcessors() const
		{
			// TODO: GetLogicalProcessorInformation 가 아닌 GetSystemInfo 를 쓰는 게 맞나?
			return m_SystemInfo.dwNumberOfProcessors;
		}

		//! \note GetModuleFileNameEx 함수를 쓰려면 프로젝트 추가 종속성에 Psapi.lib 를 추가해야 한다.
		static _txstring BaseDirectory();

	};
}