#include "stdafx.h"
#include "FileVersionInfo.h"

namespace Earlgrey
{
	FileVersionInfo::FileVersionInfo(
		const std::wstring& fileVersion
		, const std::wstring& productName
		, const std::wstring& productVersion
		, const std::wstring& fileDescription
		)
		: m_fileVersion(fileVersion)
		, m_productName(productName)
		, m_productVersion(productVersion)
		, m_fileDescription(fileDescription)
	{

	}

	FileVersionInfo FileVersionInfo::GetVersionInfo(const std::wstring& fileName)
	{
		DBG_UNREFERENCED_PARAMETER(fileName);

		using namespace std::tr1;

		// 파일로부터 버전정보데이터의 크기가 얼마인지를 구합니다.
		DWORD infoSize = GetFileVersionInfoSizeW(fileName.c_str(), 0);
		if(infoSize == 0) 
			throw std::exception("Getting version information failed!");

		// 버퍼할당
		BYTE * version = new BYTE[infoSize];
		shared_ptr<BYTE> bufferPtr(version, ArrayDeleter<BYTE>() );

		if(version == NULL)
			throw std::exception("Memory allocation failed!");

		// 버전정보데이터를 가져옵니다.
		if(GetFileVersionInfoW(fileName.c_str(), 0, infoSize, version) == 0)
			throw std::exception("Getting version information failed!");

		VS_FIXEDFILEINFO* pFileInfo = NULL;
		UINT fileInfoSize = 0;
		// buffer로 부터 VS_FIXEDFILEINFO 정보를 가져옵니다.
		if(VerQueryValueW(version, L"\\",(LPVOID*)&pFileInfo, &fileInfoSize) == 0)
			throw std::exception("Getting version informatoin failed!");


		// FileVersion 
		WORD majorVer, minorVer, buildNum, revisionNum;
		majorVer = HIWORD(pFileInfo->dwFileVersionMS);
		minorVer = LOWORD(pFileInfo->dwFileVersionMS);
		buildNum = HIWORD(pFileInfo->dwFileVersionLS);
		revisionNum = LOWORD(pFileInfo->dwFileVersionLS);

		std::wstringstream ss;
		ss << majorVer << L"." << minorVer << L"." << buildNum << L"." << revisionNum;
		std::wstring fileVersion(ss.str());


		void  * buffer = NULL;
		UINT bufLen = 0;
		DWORD * translation = NULL;

		// TRANSLATION 
		if(VerQueryValueW(version, L"\\VarFileInfo\\Translation", (LPVOID*)&translation, &bufLen) == 0)
			throw std::exception("Getting translation failed!");

		EARLGREY_ASSERT(translation != NULL);


		// PRODUCT NAME
		WCHAR path[MAX_PATH];
		wsprintfW(path, L"\\StringFileInfo\\%04x%04x\\ProductName", LOWORD(*translation), HIWORD(*translation) );

		if( ::VerQueryValueW(version, path, &buffer, &bufLen) == 0 )
			throw std::exception("Getting a product name failed!");

		EARLGREY_ASSERT(buffer != NULL);

		std::wstring productName( (LPCWSTR)buffer ); 

		// PRODUCT VERSION 
		wsprintfW(path, L"\\StringFileInfo\\%04x%04x\\ProductVersion", LOWORD(*translation), HIWORD(*translation) );

		if( ::VerQueryValueW(version, path, &buffer, &bufLen) == 0 )
			throw std::exception("Getting a product version failed!");

		EARLGREY_ASSERT(buffer != NULL);

		std::wstring productVersion( (LPCWSTR)buffer ); 

		// File Description
		wsprintfW(path, L"\\StringFileInfo\\%04x%04x\\FileDescription", LOWORD(*translation), HIWORD(*translation) );

		if( ::VerQueryValueW(version, path, &buffer, &bufLen) == 0 )
			throw std::exception("Getting a file description failed!");

		EARLGREY_ASSERT(buffer != NULL);

		std::wstring fileDescription( (LPCWSTR)buffer ); // "1, 0, 0, 1

		return FileVersionInfo(fileVersion, productName, productVersion, fileDescription);
	}


}