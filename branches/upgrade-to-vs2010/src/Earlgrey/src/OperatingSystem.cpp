#include "stdafx.h"
#include "OperatingSystem.h"

#include <strsafe.h.>

namespace Earlgrey
{
	namespace
	{
		//! \ref http://blogs.msdn.com/oldnewthing/archive/2005/02/01/364563.aspx
		BOOL Is64BitWindowsInternal()
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

		BOOL GetOSDisplayString( LPTSTR pszOS, size_t length )
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
				return FALSE; // TODO: 예외 던지는 게 나을까?

			// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

			pGNSI = (PGNSI) GetProcAddress(
				GetModuleHandle(TEXT("kernel32.dll")), 
				"GetNativeSystemInfo");
			if(NULL != pGNSI)
				pGNSI(&si);
			else GetSystemInfo(&si);

			if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && osvi.dwMajorVersion > 4 )
			{
				StringCchCopy(pszOS, length, TEXT("Microsoft "));

				// Test for the specific product.

				if ( osvi.dwMajorVersion == 6 )
				{
					// by kaistizen
					if( osvi.dwMinorVersion == 1 )
					{
						if( osvi.wProductType == VER_NT_WORKSTATION )
							StringCchCat(pszOS, length, TEXT("Windows 7 "));
						else StringCchCat(pszOS, length, TEXT("Windows Server 2008 R2 " ));
					}

					if( osvi.dwMinorVersion == 0 )
					{
						if( osvi.wProductType == VER_NT_WORKSTATION )
							StringCchCat(pszOS, length, TEXT("Windows Vista "));
						else StringCchCat(pszOS, length, TEXT("Windows Server 2008 " ));
					}

					if ( osvi.dwMinorVersion == 1 )
					{
						if( osvi.wProductType == VER_NT_WORKSTATION )
							StringCchCat(pszOS, length, TEXT("Windows 7 "));
						else StringCchCat(pszOS, length, TEXT("Windows Server 2008 R2 " ));
					}

					pGPI = (PGPI) GetProcAddress(
						GetModuleHandle(TEXT("kernel32.dll")), 
						"GetProductInfo");

					pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

					switch( dwType )
					{
					case PRODUCT_ULTIMATE:
						StringCchCat(pszOS, length, TEXT("Ultimate Edition" ));
						break;
					case PRODUCT_HOME_PREMIUM:
						StringCchCat(pszOS, length, TEXT("Home Premium Edition" ));
						break;
					case PRODUCT_HOME_BASIC:
						StringCchCat(pszOS, length, TEXT("Home Basic Edition" ));
						break;
					case PRODUCT_ENTERPRISE:
						StringCchCat(pszOS, length, TEXT("Enterprise Edition" ));
						break;
					case PRODUCT_BUSINESS:
						StringCchCat(pszOS, length, TEXT("Business Edition" ));
						break;
					case PRODUCT_STARTER:
						StringCchCat(pszOS, length, TEXT("Starter Edition" ));
						break;
					case PRODUCT_CLUSTER_SERVER:
						StringCchCat(pszOS, length, TEXT("Cluster Server Edition" ));
						break;
					case PRODUCT_DATACENTER_SERVER:
						StringCchCat(pszOS, length, TEXT("Datacenter Edition" ));
						break;
					case PRODUCT_DATACENTER_SERVER_CORE:
						StringCchCat(pszOS, length, TEXT("Datacenter Edition (core installation)" ));
						break;
					case PRODUCT_ENTERPRISE_SERVER:
						StringCchCat(pszOS, length, TEXT("Enterprise Edition" ));
						break;
					case PRODUCT_ENTERPRISE_SERVER_CORE:
						StringCchCat(pszOS, length, TEXT("Enterprise Edition (core installation)" ));
						break;
					case PRODUCT_ENTERPRISE_SERVER_IA64:
						StringCchCat(pszOS, length, TEXT("Enterprise Edition for Itanium-based Systems" ));
						break;
					case PRODUCT_SMALLBUSINESS_SERVER:
						StringCchCat(pszOS, length, TEXT("Small Business Server" ));
						break;
					case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
						StringCchCat(pszOS, length, TEXT("Small Business Server Premium Edition" ));
						break;
					case PRODUCT_STANDARD_SERVER:
						StringCchCat(pszOS, length, TEXT("Standard Edition" ));
						break;
					case PRODUCT_STANDARD_SERVER_CORE:
						StringCchCat(pszOS, length, TEXT("Standard Edition (core installation)" ));
						break;
					case PRODUCT_WEB_SERVER:
						StringCchCat(pszOS, length, TEXT("Web Server Edition" ));
						break;
					}
				}

				if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
				{
					if( GetSystemMetrics(SM_SERVERR2) )
						StringCchCat(pszOS, length, TEXT( "Windows Server 2003 R2, "));
					else if ( osvi.wSuiteMask==VER_SUITE_STORAGE_SERVER )
						StringCchCat(pszOS, length, TEXT( "Windows Storage Server 2003"));
#ifdef VER_SUITE_WH_SERVER // by kaistizen
					else if ( osvi.wSuiteMask==VER_SUITE_WH_SERVER )
						StringCchCat(pszOS, length, TEXT( "Windows Home Server"));
#endif
					else if( osvi.wProductType == VER_NT_WORKSTATION &&
						si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
					{
						StringCchCat(pszOS, length, TEXT( "Windows XP Professional x64 Edition"));
					}
					else StringCchCat(pszOS, length, TEXT("Windows Server 2003, "));

					// Test for the server type.
					if ( osvi.wProductType != VER_NT_WORKSTATION )
					{
						if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
						{
							if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
								StringCchCat(pszOS, length, TEXT( "Datacenter Edition for Itanium-based Systems" ));
							else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
								StringCchCat(pszOS, length, TEXT( "Enterprise Edition for Itanium-based Systems" ));
						}

						else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
						{
							if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
								StringCchCat(pszOS, length, TEXT( "Datacenter x64 Edition" ));
							else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
								StringCchCat(pszOS, length, TEXT( "Enterprise x64 Edition" ));
							else StringCchCat(pszOS, length, TEXT( "Standard x64 Edition" ));
						}

						else
						{
							if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
								StringCchCat(pszOS, length, TEXT( "Compute Cluster Edition" ));
							else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
								StringCchCat(pszOS, length, TEXT( "Datacenter Edition" ));
							else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
								StringCchCat(pszOS, length, TEXT( "Enterprise Edition" ));
							else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
								StringCchCat(pszOS, length, TEXT( "Web Edition" ));
							else StringCchCat(pszOS, length, TEXT( "Standard Edition" ));
						}
					}
				}

				if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
				{
					StringCchCat(pszOS, length, TEXT("Windows XP "));
					if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
						StringCchCat(pszOS, length, TEXT( "Home Edition" ));
					else StringCchCat(pszOS, length, TEXT( "Professional" ));
				}

				if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
				{
					StringCchCat(pszOS, length, TEXT("Windows 2000 "));

					if ( osvi.wProductType == VER_NT_WORKSTATION )
					{
						StringCchCat(pszOS, length, TEXT( "Professional" ));
					}
					else 
					{
						if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
							StringCchCat(pszOS, length, TEXT( "Datacenter Server" ));
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							StringCchCat(pszOS, length, TEXT( "Advanced Server" ));
						else StringCchCat(pszOS, length, TEXT( "Server" ));
					}
				}

				// Include service pack (if any) and build number.

				if( _tcslen(osvi.szCSDVersion) > 0 )
				{
					StringCchCat(pszOS, length, TEXT(" ") );
					StringCchCat(pszOS, length, osvi.szCSDVersion);
				}

				TCHAR buf[80];

				StringCchPrintf( buf, 80, TEXT(" (build %d)"), osvi.dwBuildNumber);
				StringCchCat(pszOS, length, buf);

				if ( osvi.dwMajorVersion >= 6 )
				{
					if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
						StringCchCat(pszOS, length, TEXT( ", 64-bit" ));
					else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
						StringCchCat(pszOS, length, TEXT(", 32-bit"));
				}

				return TRUE; 
			}

			else
			{  
				// printf( "This sample does not support this version of Windows.\n");
				return FALSE;
			}
		}
	}

	BOOL OperatingSystem::s_Initialized = FALSE;
	OperatingSystem::mutex_type OperatingSystem::s_InternalSyncObject;

	// const size_t OperatingSystem::OS_VERSION_STRING_LENGTH = MAX_PATH;
	TCHAR OperatingSystem::s_OSVersionString[OS_VERSION_STRING_LENGTH] = {0,};

	BOOL OperatingSystem::s_IsWin9x = FALSE;
	BOOL OperatingSystem::s_IsWinNt = FALSE;
	BOOL OperatingSystem::s_IsWin2K = FALSE;
	// BOOL OperatingSystem::s_IsWinXPSP2OrLater = FALSE;
	BOOL OperatingSystem::s_IsPostWin2K = FALSE;
	BOOL OperatingSystem::s_IsWinXp = FALSE;
	BOOL OperatingSystem::s_IsWinXpSp2 = FALSE;
	BOOL OperatingSystem::s_IsWin2k3 = FALSE;
	BOOL OperatingSystem::s_IsWin2k3Sp1 = FALSE;
	BOOL OperatingSystem::s_IsVista = FALSE;
	BOOL OperatingSystem::s_IsWin7 = FALSE;
	BOOL OperatingSystem::s_IsNtServer = FALSE;
	BOOL OperatingSystem::s_IsNtDesktop = FALSE;
	BOOL OperatingSystem::s_Is64BitWindows = FALSE;
	

	void OperatingSystem::Initialize() throw(...)
	{
		scoped_lock_type scopeLock(s_InternalSyncObject);


		s_Is64BitWindows = Is64BitWindowsInternal();


		if( !GetOSDisplayString( s_OSVersionString, OS_VERSION_STRING_LENGTH ) )
		{
			throw std::exception("Operating system unknown!");
		}
		

		OSVERSIONINFOEX osvi;
		SYSTEM_INFO si;
		//PGNSI pGNSI;
		//PGPI pGPI;
		// DWORD dwType;

		ZeroMemory(&si, sizeof(SYSTEM_INFO));
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		BOOL bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi);
		if( !bOsVersionInfoEx )
		{
			const DWORD errCode = GetLastError();
			const char * errMsg = Log::ErrorMessageA(errCode);
			throw std::exception(errMsg);
		}

		
		EARLGREY_VERIFY(osvi.wProductType == VER_NT_WORKSTATION || osvi.wProductType == VER_NT_SERVER);
		if(osvi.wProductType == VER_NT_SERVER)
			s_IsNtServer = TRUE;
		if(osvi.wProductType == VER_NT_WORKSTATION)
			s_IsNtDesktop = TRUE;


		//#define VER_PLATFORM_WIN32s             0
		//#define VER_PLATFORM_WIN32_WINDOWS      1
		//#define VER_PLATFORM_WIN32_NT           2
		EARLGREY_VERIFY(osvi.dwPlatformId != VER_PLATFORM_WIN32s);
		if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
		{
			s_IsWin9x = TRUE;
			s_Initialized = TRUE;
			return;
		}

		// in case of VER_PLATFORM_WIN32_NT

		// 4 : The operating system is Windows NT 4.0.
		// 5 : The operating system is Windows Server 2003 R2, Windows Server 2003, Windows XP, or Windows 2000.
		// 6 : The operating system is Windows Vista or Windows Server 2008.
		EARLGREY_VERIFY(osvi.dwMajorVersion != 4); // not supports NT 4.0
		EARLGREY_ASSERT(osvi.dwMajorVersion == 5 || osvi.dwMajorVersion == 6);

		s_IsWinNt = TRUE;
		s_IsWin2K = TRUE;

		// 0: The operating system is Windows Vista, Windows Server 2008, Windows 2000, Windows NT 4.0, or Windows 95.
		// which means this is Windows 2000 - 5.0 
		if(osvi.dwMinorVersion == 0) 
		{
			s_Initialized = TRUE;
			return;
		}

		s_IsPostWin2K = TRUE;

	
		// Windows XP - 5.1 
		if((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion == 1))
		{
			s_IsWinXp = TRUE;
		}

		if ((((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion == 1)) && (osvi.wServicePackMajor >= 2)) || (osvi.dwMajorVersion >= 6))
		{
			s_IsWinXpSp2 = TRUE;
		}

		// Windows Server 2003 R2 5.2 
		// Windows Server 2003 5.2 
		s_IsWin2k3 = TRUE;
		if ((((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion == 2)) && (osvi.wServicePackMajor >= 1)) || (osvi.dwMajorVersion >= 6))
		{
			s_IsWin2k3Sp1 = TRUE;
		}
		if (osvi.dwMajorVersion >= 6)
		{
			s_IsVista = TRUE;
		}
		if ((osvi.dwMajorVersion >= 7) || ((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion >= 1)))
		{
			s_IsWin7 = TRUE;
		}

		s_Initialized = TRUE;
	}

	BOOL OperatingSystem::IsWin9x()
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_IsWin9x;
	}
	
	BOOL OperatingSystem::IsWinNt()
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_IsWinNt;
	}
	
	BOOL OperatingSystem::IsWin2K()
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_IsWin2K;
	}

	BOOL OperatingSystem::IsPostWin2K()
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_IsPostWin2K;
	}

	BOOL OperatingSystem::IsWinXp()
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_IsWinXp;
	}

	BOOL OperatingSystem::IsWinXpSp2()
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_IsWinXpSp2;
	}

	BOOL OperatingSystem::IsWin2k3()
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_IsWin2k3;
	}

	BOOL OperatingSystem::IsWin2k3Sp1()
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_IsWin2k3Sp1;
	}

	BOOL OperatingSystem::IsVista()
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_IsVista;
	}

	BOOL OperatingSystem::IsWin7()
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_IsWin7;
	}

	BOOL OperatingSystem::IsNtServer()
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_IsNtServer;
	}

	BOOL OperatingSystem::IsNtDesktop()
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_IsNtDesktop;
	}

	BOOL OperatingSystem::Is64BitWindows()
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_Is64BitWindows;
	}
	
	//BOOL OperatingSystem::IsWinXPSP2OrLater()
	//{
	//	EARLGREY_ASSERT(s_Initialized);
	//	return s_IsWinXPSP2OrLater;
	//}

	const TCHAR * const OperatingSystem::OSVersion() throw(...)
	{
		EARLGREY_ASSERT(s_Initialized);
		return s_OSVersionString;
	}
}
