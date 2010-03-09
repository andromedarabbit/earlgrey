#pragma once
#include "Uncopyable.h"
#include "BasicBuffer.hpp"
#include "xvector.h"

namespace Earlgrey
{
	class IniSection;

	class IniReader : private Uncopyable
	{
	public:
		typedef basic_buffer<TCHAR> Buffer;
		typedef xvector<_txstring>::Type SectionNameContainer;
		
		explicit IniReader(const _txstring& filePath);

		inline
			const _txstring& FilePath() const 
		{
			return m_FilePath;
		}

		inline
			const SectionNameContainer& SectionNames() const 
		{
			EARLGREY_ASSERT(m_Open);
			return m_SectionNames;
		}

		BOOL Open();

		BOOL MoveSection(const _txstring& sectionName);

		BOOL MoveNextSection();

		IniSection CurrentSection() const;

		IniSection operator[] (const _txstring& sectionName) const;
		

	private: // methods
		BOOL ReadSectionNames();

	private: // fields
		Buffer m_Buffer;
		_txstring m_FilePath;
		BOOL m_Open;
		SectionNameContainer m_SectionNames;
		int m_NextSection; // TODO: 이해하기 쉽게 m_CurrentSection으로 바꾸면 좋겠다.

	};
}