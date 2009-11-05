#pragma once
#pragma comment(lib,"version.lib")
// #include "Uncopyable.h"

#include "txstring.h"
#include "RAII.h"

// #include "tiostream.h"
#include "tsstream.h"

namespace Earlgrey
{
	class FileVersionInfo//  : private Uncopyable
	{
	private:
		explicit FileVersionInfo(
			const _txstring& fileVersion
			, const _txstring& productName
			, const _txstring& productVersion
			, const _txstring& fileDescription
			)
			: m_fileVersion(fileVersion)
			, m_productName(productName)
			, m_productVersion(productVersion)
			, m_fileDescription(fileDescription)
		{

		}

	public: // class methods
		static FileVersionInfo GetVersionInfo(const _txstring& fileName)
		{
			DBG_UNREFERENCED_PARAMETER(fileName);

			using namespace std::tr1;

			// 파일로부터 버전정보데이터의 크기가 얼마인지를 구합니다.
			DWORD infoSize = GetFileVersionInfoSize(fileName.c_str(), 0);
			if(infoSize == 0) 
				throw std::exception("Getting version information failed!");

			// 버퍼할당
			BYTE * version = new BYTE[infoSize];
			shared_ptr<BYTE> bufferPtr(version, ArrayDeleter<BYTE>() );

			if(version == NULL)
				throw std::exception("Memory allocation failed!");

			// 버전정보데이터를 가져옵니다.
			if(GetFileVersionInfo(fileName.c_str(), 0, infoSize, version) == 0)
				throw std::exception("Getting version information failed!");

			VS_FIXEDFILEINFO* pFileInfo = NULL;
			UINT fileInfoSize = 0;
			// buffer로 부터 VS_FIXEDFILEINFO 정보를 가져옵니다.
			if(VerQueryValue(version, _T("\\"),(LPVOID*)&pFileInfo, &fileInfoSize) == 0)
				throw std::exception("Getting version information failed!");


			// FileVersion 
			WORD majorVer, minorVer, buildNum, revisionNum;
			majorVer = HIWORD(pFileInfo->dwFileVersionMS);
			minorVer = LOWORD(pFileInfo->dwFileVersionMS);
			buildNum = HIWORD(pFileInfo->dwFileVersionLS);
			revisionNum = LOWORD(pFileInfo->dwFileVersionLS);

			_txstringstream ss;
			ss << majorVer << _T(".") << minorVer << _T(".") << buildNum << _T(".") << revisionNum;
			_txstring fileVersion(ss.str());


			void  * buffer = NULL;
			UINT bufLen = 0;
			DWORD * translation = NULL;

			// TRANSLATION 
			if(VerQueryValue(version, _T("\\VarFileInfo\\Translation"), (LPVOID*)&translation, &bufLen) == 0)
				throw std::exception("Getting translation failed!");

			EARLGREY_ASSERT(translation != NULL);


			// PRODUCT NAME
			TCHAR path[MAX_PATH];
			wsprintf(path, _T("\\StringFileInfo\\%04x%04x\\ProductName"), LOWORD(*translation), HIWORD(*translation) );

			if( ::VerQueryValue(version, path, &buffer, &bufLen) == 0 )
				throw std::exception("Getting a product name failed!");

			EARLGREY_ASSERT(buffer != NULL);

			_txstring productName( (LPCTSTR)buffer ); 

			// PRODUCT VERSION 
			wsprintf(path, _T("\\StringFileInfo\\%04x%04x\\ProductVersion"), LOWORD(*translation), HIWORD(*translation) );

			if( ::VerQueryValue(version, path, &buffer, &bufLen) == 0 )
				throw std::exception("Getting a product version failed!");

			EARLGREY_ASSERT(buffer != NULL);

			_txstring productVersion( (LPCTSTR)buffer ); 

			// File Description
			wsprintf(path, _T("\\StringFileInfo\\%04x%04x\\FileDescription"), LOWORD(*translation), HIWORD(*translation) );

			if( ::VerQueryValue(version, path, &buffer, &bufLen) == 0 )
				throw std::exception("Getting a file description failed!");

			EARLGREY_ASSERT(buffer != NULL);

			_txstring fileDescription( (LPCTSTR)buffer ); // "1, 0, 0, 1

			return FileVersionInfo(fileVersion, productName, productVersion, fileDescription);
		}


	public: // instance methods
		inline _txstring& FileVersion() 
		{
			return m_fileVersion;
		}
		inline const _txstring& FileVersion() const 
		{
			return m_fileVersion;
		}
	
		inline _txstring& ProductName() 
		{
			return m_productName;
		}
		inline const _txstring& ProductName() const 
		{
			return m_productName;
		}

		inline _txstring& ProductVersion() 
		{
			return m_productVersion;
		}
		inline const _txstring& ProductVersion() const 
		{
			return m_productVersion;
		}

		inline _txstring& FileDescription() 
		{
			return m_fileDescription;
		}
		inline const _txstring& FileDescription() const 
		{
			return m_fileDescription;
		}

	private:
		_txstring m_fileVersion;
		_txstring m_productName;
		_txstring m_productVersion;
		_txstring m_fileDescription;

	};

}