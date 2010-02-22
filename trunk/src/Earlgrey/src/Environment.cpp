#include "stdafx.h"
#include "Environment.h"
#include "Path.h"

#include <Psapi.h> // GetModuleFileNameEx
#include <Lmcons.h> // UNLEN
#include <strsafe.h> // StringCchCat, ... 

#pragma comment(lib, "Psapi.lib")

namespace Earlgrey 
{ 
	const TCHAR* const Environment::NewLine()
	{
		return TEXT("\r\n"); // 현재는 Windows만 지원하므로,
	}

	DWORD Environment::ProcessorCount()
	{	
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		return sysinfo.dwNumberOfProcessors;	
	}

	//! \ref http://www.tipssoft.com/bulletin/board.php?bo_table=QnA&wr_id=2395
	_txstring Environment::BaseDirectory()
	{
		const DWORD bufferSize = MAX_PATH + 1;

		TCHAR modName[bufferSize];
		if(GetModuleFileNameEx(GetCurrentProcess(), NULL, modName, _countof(modName)) == 0)
		{
			// \todo 오류 처리 - GetLastError

		}

		_txstring directory(modName);
		return Path::GetDirectoryName(directory);
	}

	_txstring Environment::CurrentDirectory()
	{
		const DWORD bufferSize = MAX_PATH + 1;

		TCHAR dirName[bufferSize]; 
		if(GetCurrentDirectory(bufferSize, dirName) == 0)
		{
			// \todo 오류 처리 - GetLastError

		}
	
		_txstring directory(dirName);
		return directory;
	}

	//! 현재는 throw 처리를 하는데 그럴거면 BOOL가 아닌 void 반환이 나을 듯. GetLastError로 오류를 확인하면 되니까.
	BOOL Environment::CurrentDirectory(const _txstring& pathName) throw(...)
	{
		if(pathName.empty())
		{
			throw std::invalid_argument("Path shouldn't be an empty string!");
		}

		if(pathName.length() > MAX_PATH)
		{
			throw std::invalid_argument("The length of the directory path is too long!");
		}

		return SetCurrentDirectory(pathName.c_str());
	}

	_txstring Environment::MachineName()
	{
		const DWORD bufferSize = MAX_COMPUTERNAME_LENGTH + 1;

		TCHAR name[bufferSize]; 
		
		DWORD size = bufferSize;
		if(GetComputerName(name, &size) == 0)
		{
			// \todo 오류 처리 - GetLastError

		}

		return name;
	}

	_txstring Environment::UserDomainName()
	{
		const DWORD bufferSize = UNLEN  + 1;

		TCHAR name[bufferSize]; 

		DWORD size = bufferSize;
		if(GetUserName(name, &size) == 0)
		{
			// \todo 오류 처리 - GetLastError

		}
	
		return name;
	}

	
	//! \ref http://msdn2.microsoft.com/en-us/library/ms724429.aspx
	//! \ref http://msdn.microsoft.com/en-us/library/ms724834%28VS.85%29.aspx
	static const int BUFSIZE = 256;

	BOOL GetOSDisplayString( LPTSTR pszOS )
	{
		typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
		typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);


		OSVERSIONINFOEX osvi;
		SYSTEM_INFO si;
		PGNSI pGNSI;
		PGPI pGPI;
		DWORD dwType;

		ZeroMemory(&si, sizeof(SYSTEM_INFO));
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		BOOL bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi);
		if( !bOsVersionInfoEx )
			return 1;

		// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

		pGNSI = (PGNSI) GetProcAddress(
			GetModuleHandle(TEXT("kernel32.dll")), 
			"GetNativeSystemInfo");
		if(NULL != pGNSI)
			pGNSI(&si);
		else GetSystemInfo(&si);

		if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && 
			osvi.dwMajorVersion > 4 )
		{
			StringCchCopy(pszOS, BUFSIZE, TEXT("Microsoft "));

			// Test for the specific product.

			if ( osvi.dwMajorVersion == 6 )
			{
				// by kaistizen
				if( osvi.dwMinorVersion == 1 )
				{
					if( osvi.wProductType == VER_NT_WORKSTATION )
						StringCchCat(pszOS, BUFSIZE, TEXT("Windows 7 "));
					else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 R2 " ));
				}

				if( osvi.dwMinorVersion == 0 )
				{
					if( osvi.wProductType == VER_NT_WORKSTATION )
						StringCchCat(pszOS, BUFSIZE, TEXT("Windows Vista "));
					else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 " ));
				}

				if ( osvi.dwMinorVersion == 1 )
				{
					if( osvi.wProductType == VER_NT_WORKSTATION )
						StringCchCat(pszOS, BUFSIZE, TEXT("Windows 7 "));
					else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 R2 " ));
				}

				pGPI = (PGPI) GetProcAddress(
					GetModuleHandle(TEXT("kernel32.dll")), 
					"GetProductInfo");

				pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

				switch( dwType )
				{
				case PRODUCT_ULTIMATE:
					StringCchCat(pszOS, BUFSIZE, TEXT("Ultimate Edition" ));
					break;
				case PRODUCT_HOME_PREMIUM:
					StringCchCat(pszOS, BUFSIZE, TEXT("Home Premium Edition" ));
					break;
				case PRODUCT_HOME_BASIC:
					StringCchCat(pszOS, BUFSIZE, TEXT("Home Basic Edition" ));
					break;
				case PRODUCT_ENTERPRISE:
					StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
					break;
				case PRODUCT_BUSINESS:
					StringCchCat(pszOS, BUFSIZE, TEXT("Business Edition" ));
					break;
				case PRODUCT_STARTER:
					StringCchCat(pszOS, BUFSIZE, TEXT("Starter Edition" ));
					break;
				case PRODUCT_CLUSTER_SERVER:
					StringCchCat(pszOS, BUFSIZE, TEXT("Cluster Server Edition" ));
					break;
				case PRODUCT_DATACENTER_SERVER:
					StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition" ));
					break;
				case PRODUCT_DATACENTER_SERVER_CORE:
					StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition (core installation)" ));
					break;
				case PRODUCT_ENTERPRISE_SERVER:
					StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
					break;
				case PRODUCT_ENTERPRISE_SERVER_CORE:
					StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition (core installation)" ));
					break;
				case PRODUCT_ENTERPRISE_SERVER_IA64:
					StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition for Itanium-based Systems" ));
					break;
				case PRODUCT_SMALLBUSINESS_SERVER:
					StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server" ));
					break;
				case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
					StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server Premium Edition" ));
					break;
				case PRODUCT_STANDARD_SERVER:
					StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition" ));
					break;
				case PRODUCT_STANDARD_SERVER_CORE:
					StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition (core installation)" ));
					break;
				case PRODUCT_WEB_SERVER:
					StringCchCat(pszOS, BUFSIZE, TEXT("Web Server Edition" ));
					break;
				}
			}

			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
			{
				if( GetSystemMetrics(SM_SERVERR2) )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Server 2003 R2, "));
				else if ( osvi.wSuiteMask==VER_SUITE_STORAGE_SERVER )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Storage Server 2003"));
#ifdef VER_SUITE_WH_SERVER // by kaistizen
				else if ( osvi.wSuiteMask==VER_SUITE_WH_SERVER )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Home Server"));
#endif
				else if( osvi.wProductType == VER_NT_WORKSTATION &&
					si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
				{
					StringCchCat(pszOS, BUFSIZE, TEXT( "Windows XP Professional x64 Edition"));
				}
				else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2003, "));

				// Test for the server type.
				if ( osvi.wProductType != VER_NT_WORKSTATION )
				{
					if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
					{
						if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
							StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition for Itanium-based Systems" ));
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition for Itanium-based Systems" ));
					}

					else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
					{
						if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
							StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter x64 Edition" ));
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise x64 Edition" ));
						else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard x64 Edition" ));
					}

					else
					{
						if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
							StringCchCat(pszOS, BUFSIZE, TEXT( "Compute Cluster Edition" ));
						else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
							StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition" ));
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition" ));
						else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
							StringCchCat(pszOS, BUFSIZE, TEXT( "Web Edition" ));
						else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard Edition" ));
					}
				}
			}

			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
			{
				StringCchCat(pszOS, BUFSIZE, TEXT("Windows XP "));
				if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Home Edition" ));
				else StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
			}

			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
			{
				StringCchCat(pszOS, BUFSIZE, TEXT("Windows 2000 "));

				if ( osvi.wProductType == VER_NT_WORKSTATION )
				{
					StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
				}
				else 
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Server" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Advanced Server" ));
					else StringCchCat(pszOS, BUFSIZE, TEXT( "Server" ));
				}
			}

			// Include service pack (if any) and build number.

			if( _tcslen(osvi.szCSDVersion) > 0 )
			{
				StringCchCat(pszOS, BUFSIZE, TEXT(" ") );
				StringCchCat(pszOS, BUFSIZE, osvi.szCSDVersion);
			}

			TCHAR buf[80];

			StringCchPrintf( buf, 80, TEXT(" (build %d)"), osvi.dwBuildNumber);
			StringCchCat(pszOS, BUFSIZE, buf);

			if ( osvi.dwMajorVersion >= 6 )
			{
				if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
					StringCchCat(pszOS, BUFSIZE, TEXT( ", 64-bit" ));
				else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
					StringCchCat(pszOS, BUFSIZE, TEXT(", 32-bit"));
			}

			return TRUE; 
		}

		else
		{  
			// printf( "This sample does not support this version of Windows.\n");
			return FALSE;
		}
	}

	_txstring Environment::OSVersion()
	{
		TCHAR pszOS[BUFSIZE];
		if( !GetOSDisplayString( pszOS ) )
		{
			//! \todo 오류 처리

		}
		_txstring osVersion(pszOS);
		return osVersion;
	}


	//! \ref http://msdn.microsoft.com/en-us/library/ms725491%28VS.85%29.aspx
	BOOL Environment::Is_WinXP_SP2_or_Later() 
	{
		OSVERSIONINFOEX osvi;
		DWORDLONG dwlConditionMask = 0;
		BYTE op = VER_GREATER_EQUAL;

		// Initialize the OSVERSIONINFOEX structure.

		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		osvi.dwMajorVersion = 5;
		osvi.dwMinorVersion = 1;
		osvi.wServicePackMajor = 2;
		osvi.wServicePackMinor = 0;

		// Initialize the condition mask.

		VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, op );
		VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, op );
		VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMAJOR, op );
		VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMINOR, op );

		// Perform the test.

		return VerifyVersionInfo(
			&osvi, 
			VER_MAJORVERSION | VER_MINORVERSION | 
			VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR,
			dwlConditionMask);
	}

	BOOL Environment::Is_Win_Server() 
	{
		OSVERSIONINFOEX osvi;
		DWORDLONG dwlConditionMask = 0;
		BYTE op = VER_GREATER_EQUAL;

		// Initialize the OSVERSIONINFOEX structure.

		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		osvi.dwMajorVersion = 5;
		osvi.dwMinorVersion = 0;
		osvi.wServicePackMajor = 0;
		osvi.wServicePackMinor = 0;
		osvi.wProductType = VER_NT_SERVER;

		// Initialize the condition mask.

		VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, op );
		VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, op );
		VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMAJOR, op );
		VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMINOR, op );
		VER_SET_CONDITION( dwlConditionMask, VER_PRODUCT_TYPE, VER_EQUAL );

		// Perform the test.

		return VerifyVersionInfo(
			&osvi, 
			VER_MAJORVERSION | VER_MINORVERSION | 
			VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR |
			VER_PRODUCT_TYPE,
			dwlConditionMask);
	}

	//! \ref http://blogs.msdn.com/oldnewthing/archive/2005/02/01/364563.aspx
	BOOL Environment::Is64BitWindows()
	{
#if defined(_WIN64)
		return TRUE;  // 64-bit programs run only on Win64
#elif defined(_WIN32)
		// 32-bit programs run on both 32-bit and 64-bit Windows
		// so must sniff
		BOOL f64 = FALSE;
		return IsWow64Process(GetCurrentProcess(), &f64) && f64;
#else
		return FALSE; // Win64 does not support Win16
#endif
	}

	//! \ref Hard code vol.1
	BOOL Environment::IsLittleEndian()
	{
		union 
		{
			unsigned long bits32;
			unsigned char bytes[4];
		} theValue;

		theValue.bytes[0] = 0;
		theValue.bytes[1] = 1;
		theValue.bytes[2] = 0;
		theValue.bytes[3] = 0;

		return theValue.bits32 == 256;
	}


	// TODO: 메모리 해제를 스마트 포인터로...
	namespace 
	{
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION GetSystemLogicalProcessorInformation(
			DWORD& returnLength
			)
		{
			PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL; //, ptr;
			returnLength = 0;

			for( ; ; )
			{
				if( ::GetLogicalProcessorInformation (buffer, &returnLength) == TRUE )
					break;

				const DWORD errorCode = GetLastError();
				EARLGREY_VERIFY(errorCode == ERROR_INSUFFICIENT_BUFFER);

				if (errorCode == ERROR_INSUFFICIENT_BUFFER) 
				{
					if (buffer) 
						::free(buffer);

					buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)::malloc(returnLength);
					EARLGREY_VERIFY(buffer != NULL); // Allocation failure
				} 
			}

			return buffer;
		}
	}

	WORD Environment::ProcessorCacheLineSize()
	{
		DWORD returnLength = 0;
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = GetSystemLogicalProcessorInformation(returnLength);
		EARLGREY_ASSERT(buffer != NULL);

		WORD cacheLineSize = 0;
		DWORD byteOffset = 0;

		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = buffer;
		while (byteOffset < returnLength) 
		{
			switch (ptr->Relationship) 
			{
			case RelationCache:
				cacheLineSize = ptr->Cache.LineSize;
				::free(buffer);
				return cacheLineSize;

			default:
				break;
			}
			byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
			ptr++;
		}

		::free(buffer);
		throw std::exception("");
		// ::free(buffer);
		// return procCoreCount ;
	}


	DWORD Environment::ActiveProcessorCoresCount()
	{
		DWORD returnLength = 0;
		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = GetSystemLogicalProcessorInformation(returnLength);
		EARLGREY_ASSERT(buffer != NULL);

		DWORD procCoreCount = 0;
		DWORD byteOffset = 0;

		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = buffer;
		while (byteOffset < returnLength) 
		{
			switch (ptr->Relationship) 
			{
			case RelationProcessorCore:
				procCoreCount++;
				break;

			default:
				break;
			}
			byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
			ptr++;
		}

		::free(buffer);
		return procCoreCount ;
	}
}
