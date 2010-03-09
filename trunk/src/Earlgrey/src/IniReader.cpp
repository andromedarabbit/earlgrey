#include "stdafx.h"
#include "IniReader.h"

#include "File.h"
#include "numeric_cast.hpp"
#include "IniSection.h"

namespace Earlgrey
{
	IniReader::IniReader(const _txstring& filePath)
		: m_Buffer()
		, m_FilePath(filePath)
		, m_Open(FALSE)
		, m_SectionNames()
		, m_NextSection(0)
	{
		m_Buffer.resize(128, _T('\0'));
	}


	BOOL IniReader::Open()
	{
		EARLGREY_ASSERT(m_FilePath.empty() == false);
		if(m_Open)
			throw std::exception("");

		if(File::Exists(m_FilePath) == FALSE)
			return FALSE;

		if(ReadSectionNames() == FALSE)
			return FALSE;

		m_Open = TRUE;
		return m_Open;
	}

	namespace 
	{
		template <typename T>
		DWORD ReadSectionNamesIntoBuffer(const TCHAR * filePath, T& m_Buffer) //, xvector<_txstring>::Type& sectionNames)
		{
			m_Buffer.clear();

			DWORD length = 0;
			for(;;)
			{
				LPTSTR rawBuffer = m_Buffer.data();
				const DWORD size = EARLGREY_NUMERIC_CAST<DWORD>(m_Buffer.capacity());

				length = GetPrivateProfileSectionNames(rawBuffer, size, filePath);
				if(length != size - 2)
					break;

				m_Buffer.reserve(size * 2);
			}

			return length;
		}
	}

	BOOL IniReader::ReadSectionNames()
	{
		const DWORD length = ReadSectionNamesIntoBuffer(m_FilePath.c_str(), m_Buffer);
		if(length < 1)
			return FALSE;

		// TODO: 아래 코드를 다듬었으면...
		LPTSTR rawBuffer = m_Buffer.data();
		LPTSTR begin = rawBuffer;

		size_t count = 0;
		for(size_t i = 0; i < length; i++)
		{
			const TCHAR firstChar = rawBuffer[i];				
			if(firstChar != _T('\0'))
			{
				count++;
				continue;
			}

			_txstring sectionName(begin, count);

			m_SectionNames.push_back(sectionName);
			begin = rawBuffer + i + 1;
			count = 0;

			const TCHAR secondChar = rawBuffer[i + 1];
			if(firstChar == _T('\0') && secondChar == _T('\0'))
				break;
		}

		return TRUE;
	}

	namespace
	{
		size_t FindNextSectionIndex(const IniReader::SectionNameContainer& sectionNames, const _txstring& sectionName)
		{
			for(size_t i = 0; i < sectionNames.size(); i++)
			{
				if(sectionNames[i] == sectionName)
				{
					return i + 1;
				}
			}
			return 0;
		}
	}

	BOOL IniReader::MoveSection(const _txstring& sectionName)
	{
		size_t i = FindNextSectionIndex(m_SectionNames, sectionName);
		if(i == 0)
			return FALSE;

		m_NextSection = i;
		return TRUE;
	}

	BOOL IniReader::MoveNextSection()
	{
		if(m_SectionNames.size() < m_NextSection + static_cast<size_t>(1))
			return FALSE;

		m_NextSection++;
		return TRUE;
	}

	IniSection IniReader::CurrentSection() const 
	{
		return IniSection(m_FilePath, m_SectionNames[m_NextSection - 1]) ;
	}

 	IniSection IniReader::operator[] (const _txstring& sectionName) const
 	{
		size_t i = FindNextSectionIndex(m_SectionNames, sectionName);
		if(i == 0)
 			throw std::exception("");
 
 		return IniSection(m_FilePath, m_SectionNames[i - 1]);
 	}

}