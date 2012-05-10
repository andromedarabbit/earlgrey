#pragma once
#pragma comment(lib,"version.lib")

#include "Uncopyable.h"
#include "tstring.h"
#include "tsstream.h"
#include "RAII.h"

namespace Earlgrey
{
	class FileVersionInfo : private Uncopyable
	{
	private:
		explicit FileVersionInfo(
			const std::wstring& fileVersion
			, const std::wstring& productName
			, const std::wstring& productVersion
			, const std::wstring& fileDescription
			);

	public: // class methods
		static FileVersionInfo GetVersionInfo(const std::wstring& fileName);


	public: // instance methods
		inline std::wstring& FileVersion() 
		{
			return m_fileVersion;
		}
		inline const std::wstring& FileVersion() const 
		{
			return m_fileVersion;
		}
	
		inline std::wstring& ProductName() 
		{
			return m_productName;
		}
		inline const std::wstring& ProductName() const 
		{
			return m_productName;
		}

		inline std::wstring& ProductVersion() 
		{
			return m_productVersion;
		}
		inline const std::wstring& ProductVersion() const 
		{
			return m_productVersion;
		}

		inline std::wstring& FileDescription() 
		{
			return m_fileDescription;
		}
		inline const std::wstring& FileDescription() const 
		{
			return m_fileDescription;
		}

	private:
		std::wstring m_fileVersion;
		std::wstring m_productName;
		std::wstring m_productVersion;
		std::wstring m_fileDescription;

	};

}