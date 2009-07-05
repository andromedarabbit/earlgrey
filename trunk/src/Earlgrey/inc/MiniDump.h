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
		/// ������ �������� ����
		enum DumpLevel
		{
			DUMP_LEVEL_LIGHT,  ///< MiniDumpNormal�� ���
			DUMP_LEVEL_MEDIUM, ///< MiniDumpWithDataSegs�� ���
			DUMP_LEVEL_HEAVY   ///< MiniDumpWithFullMemory�� ���
		};


	private:
		DumpLevel s_DumpLevel;          ///< ���� ����.
		static bool      s_AddTimeStamp;       ///< ��¥ ��� ���� ���� �̸� ���
		static TCHAR     s_AppName[_MAX_PATH]; ///< ���� ���� �̸�
		static TCHAR     s_CallStack[8192];    ///< �ݽ��� ���ڿ�
		static TCHAR     s_Modules[8192];      ///< ��� ���ڿ�
		static LPCTSTR   s_DialogTemplate;     ///< ��ȭâ ���ø�
		static DLGPROC   s_DialogProc;         ///< ��ȭâ ���ν���


	public:
		/// \brief �̴� ���� ����� �ʱ�ȭ�Ѵ�.
		static void Install(DumpLevel dumpLevel, bool addTimestamp=true, 
			LPCTSTR dialogTemplate=NULL, DLGPROC dialogProc=NULL);

		/// \brief �ݽ��� ���ڿ��� ��ȯ�Ѵ�.
		static LPCTSTR GetCallStack() { return s_CallStack; }

		/// \brief ��� ���ڿ��� ��ȯ�Ѵ�.
		static LPCTSTR GetModules() { return s_Modules; }


	private:
		/// \brief ���ܿ� ���� ������ �޾Ƽ�, �̴� ���� ������ �����Ѵ�. 
		static LONG WINAPI WriteDump(PEXCEPTION_POINTERS exPtrs);

		/// \brief ���� ����
		cMiniDump() {}

		/// \brief ���� ���� ����
		cMiniDump(const cMiniDump&) {}

		/// \brief ���� ���� ����
		cMiniDump& operator = (const cMiniDump&) { return *this; }


	};
	
	*/


	//! \todo �ڵ� ¥��!
	class MiniDump : private Uncopyable
	{
		friend struct Loki::CreateStatic<MiniDump>;
	};

	typedef 
		Loki::SingletonHolder<MiniDump, Loki::CreateStatic, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
		MiniDumpSingleton
		;
}