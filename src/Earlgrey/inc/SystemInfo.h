#pragma once
#include "Uncopyable.h"
#include "StlCustom.h"

namespace Earlgrey
{
	class SystemInfo : private Uncopyable
	{
	// private:
		// explicit SystemInfo(); // �������� �ʴ´�.

	private:
		SYSTEM_INFO m_SystemInfo;

	public:
		explicit SystemInfo()
		{
			GetSystemInfo(&m_SystemInfo);
		}

		// TODO: ���� Assert�� ��� ���� ¥����.
		/*
		void ValidateSystem() const
		{			
			WORD processorArchitecture = m_SystemInfo.wProcessorArchitecture;

			BYTE processorRivision = HIBYTE(m_SystemInfo.wProcessorRevision );
			if(processorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				return;

			if(processorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
			{
				// �ϴ� Core Duo 2 �̻� ����ó���ϴ� ���� ���� ������?
				return;
			}

			// crash
		}
		*/

		DWORD NumberOfProcessors() const
		{
			// TODO: GetLogicalProcessorInformation �� �ƴ� GetSystemInfo �� ���� �� �³�?
			return m_SystemInfo.dwNumberOfProcessors;
		}

		//! \note GetModuleFileNameEx �Լ��� ������ ������Ʈ �߰� ���Ӽ��� Psapi.lib �� �߰��ؾ� �Ѵ�.
		static _txstring BaseDirectory();

	};
}