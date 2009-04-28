#pragma 
#include "Uncopyable.h"
#include "Log.h"

namespace Earlgrey
{
	class ProcessInitializer : private Uncopyable
	{
		/*
		#define NORMAL_PRIORITY_CLASS             0x00000020
		#define IDLE_PRIORITY_CLASS               0x00000040
		#define HIGH_PRIORITY_CLASS               0x00000080
		#define REALTIME_PRIORITY_CLASS           0x00000100
		*/

		/*
		ABOVE_NORMAL_PRIORITY_CLASS	0x00008000
		--> Windows NT and Windows Me/98/95:  This value is not supported.
		BELOW_NORMAL_PRIORITY_CLASS	0x00004000
		--> Windows NT and Windows Me/98/95:  This value is not supported.
		PROCESS_MODE_BACKGROUND_BEGIN 0x00100000
		--> Windows Server 2003 and Windows XP/2000:  This value is not supported.
		PROCESS_MODE_BACKGROUND_END 0x00200000
		--> Windows Server 2003 and Windows XP/2000:  This value is not supported.
		*/
		static const DWORD DEFAULT_PRIORITY_CLASS = NORMAL_PRIORITY_CLASS;

	public:
		explicit ProcessInitializer()
			// : m_CurrentProrityClass(DEFAULT_PRIORITY_CLASS)
		{
		}

		BOOL Run()
		{
			if( SetPriorityClass(GetCurrentProcess(), DEFAULT_PRIORITY_CLASS) != 0)
			{
				// \todo 뭔가 오류 처리가 필요하다.
				DWORD errCode = GetLastError();

				_txstring errMsg = Log::LastErrorMessage(errCode);
				DBG_UNREFERENCED_LOCAL_VARIABLE(errMsg);
				
				return FALSE;
			}

			return TRUE;
		}

		inline DWORD CurrentProrityClass() const
		{
			// return m_CurrentProrityClass;
			DWORD priorityClass = GetPriorityClass(GetCurrentProcess());
			if( priorityClass != 0)
			{
				// \todo 뭔가 오류 처리가 필요하다.
			}
			return priorityClass;
		}

	private:
		// DWORD m_CurrentProrityClass;
	};
}