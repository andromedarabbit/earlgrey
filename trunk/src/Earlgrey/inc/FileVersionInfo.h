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
		explicit FileVersionInfo(_txstring fileVersion)
			: m_fileVersion(fileVersion)
		{

		}

	public: // class methods
		static FileVersionInfo GetVersionInfo(const _txstring& fileName)
		{
			DBG_UNREFERENCED_PARAMETER(fileName);

			using namespace std::tr1;

			// ���Ϸκ��� ���������������� ũ�Ⱑ �������� ���մϴ�.
			DWORD infoSize = GetFileVersionInfoSize(fileName.c_str(), 0);
			if(infoSize == 0) 
				throw std::exception("Getting version information failed!");

			// �����Ҵ�
			BYTE * buffer = new BYTE[infoSize];
			shared_ptr<BYTE> bufferPtr(buffer, ArrayDeleter<BYTE>() );

			if(buffer == NULL)
				throw std::exception("Memory allocation failed!");

			// �������������͸� �����ɴϴ�.
			if(GetFileVersionInfo(fileName.c_str(), 0, infoSize, buffer) == 0)
				throw std::exception("Getting version information failed!");

			VS_FIXEDFILEINFO* pFineInfo = NULL;
			UINT bufLen = 0;
			// buffer�� ���� VS_FIXEDFILEINFO ������ �����ɴϴ�.
			if(VerQueryValue(buffer, _T("\\"),(LPVOID*)&pFineInfo, &bufLen) == 0)
				throw std::exception("Getting version information failed!");


			// FileVersion 
			WORD majorVer, minorVer, buildNum, revisionNum;
			majorVer = HIWORD(pFineInfo->dwFileVersionMS);
			minorVer = LOWORD(pFineInfo->dwFileVersionMS);
			buildNum = HIWORD(pFineInfo->dwFileVersionLS);
			revisionNum = LOWORD(pFineInfo->dwFileVersionLS);

			_txstringstream ss;
			ss << majorVer << _T(".") << minorVer << _T(".") << buildNum << _T(".") << revisionNum;
			_txstring fileVersion(ss.str());

			// ���Ϲ��� ���
			// printf("version : %d,%d,%d,%d\n",majorVer, minorVer, buildNum, revisionNum);

			return FileVersionInfo(fileVersion);
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
	

	private:
		_txstring m_fileVersion;

	};

}