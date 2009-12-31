#pragma once
#pragma comment(lib,"version.lib")

#include "tstring.h"
#include "tsstream.h"
#include "RAII.h"

namespace Earlgrey
{
	class FileVersionInfo : private Uncopyable
	{
	private:
		explicit FileVersionInfo(
			const _tstring& fileVersion
			, const _tstring& productName
			, const _tstring& productVersion
			, const _tstring& fileDescription
			);

	public: // class methods
		static FileVersionInfo GetVersionInfo(const _tstring& fileName);


	public: // instance methods
		inline _tstring& FileVersion() 
		{
			return m_fileVersion;
		}
		inline const _tstring& FileVersion() const 
		{
			return m_fileVersion;
		}
	
		inline _tstring& ProductName() 
		{
			return m_productName;
		}
		inline const _tstring& ProductName() const 
		{
			return m_productName;
		}

		inline _tstring& ProductVersion() 
		{
			return m_productVersion;
		}
		inline const _tstring& ProductVersion() const 
		{
			return m_productVersion;
		}

		inline _tstring& FileDescription() 
		{
			return m_fileDescription;
		}
		inline const _tstring& FileDescription() const 
		{
			return m_fileDescription;
		}

	private:
		_tstring m_fileVersion;
		_tstring m_productName;
		_tstring m_productVersion;
		_tstring m_fileDescription;

	};

}