#pragma once 
#include "Uncopyable.h"
#include <Loki/Singleton.h>
#include <Loki/Threads.h> // for Loki::SingleThreaded
#include "NoLock.h"

#ifndef _WINDOWS_
#define NOMINMAX
#include <windows.h>
#endif


// #pragma comment(lib, "dbghelp")

namespace Earlgrey
{
	/*
	class MiniDump : private Uncopyable
	{
		friend struct Loki::CreateStatic<MiniDump>;

	public:
		/// 덤프할 데이터의 수준
		enum DumpLevel
		{
			DUMP_LEVEL_LIGHT,  ///< MiniDumpNormal을 사용
			DUMP_LEVEL_MEDIUM, ///< MiniDumpWithDataSegs를 사용
			DUMP_LEVEL_HEAVY   ///< MiniDumpWithFullMemory를 사용
		};


	private:
		DumpLevel s_DumpLevel;          ///< 덤프 레벨.
		static bool      s_AddTimeStamp;       ///< 날짜 기반 덤프 파일 이름 사용
		static TCHAR     s_AppName[_MAX_PATH]; ///< 덤프 파일 이름
		static TCHAR     s_CallStack[8192];    ///< 콜스택 문자열
		static TCHAR     s_Modules[8192];      ///< 모듈 문자열
		static LPCTSTR   s_DialogTemplate;     ///< 대화창 템플릿
		static DLGPROC   s_DialogProc;         ///< 대화창 프로시져


	public:
		/// \brief 미니 덤프 기능을 초기화한다.
		static void Install(DumpLevel dumpLevel, bool addTimestamp=true, 
			LPCTSTR dialogTemplate=NULL, DLGPROC dialogProc=NULL);

		/// \brief 콜스택 문자열을 반환한다.
		static LPCTSTR GetCallStack() { return s_CallStack; }

		/// \brief 모듈 문자열을 반환한다.
		static LPCTSTR GetModules() { return s_Modules; }


	private:
		/// \brief 예외에 대한 정보를 받아서, 미니 덤프 파일을 생성한다. 
		static LONG WINAPI WriteDump(PEXCEPTION_POINTERS exPtrs);

		/// \brief 생성 금지
		cMiniDump() {}

		/// \brief 복사 생성 금지
		cMiniDump(const cMiniDump&) {}

		/// \brief 대입 연산 금지
		cMiniDump& operator = (const cMiniDump&) { return *this; }


	};
	
	*/


	//! \todo 코드 짜자!
	class MiniDump : private Uncopyable
	{
		friend struct Loki::CreateStatic<MiniDump>;
	};

	typedef 
		Loki::SingletonHolder<MiniDump, Loki::CreateStatic, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
		MiniDumpSingleton
		;
}