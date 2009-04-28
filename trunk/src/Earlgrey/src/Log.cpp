#include "stdafx.h"
#include "Log.h"
#include "EarlgreyAssert.h"

namespace 
{
	//! \note 임시 조치
	template<typename BufferType>
	class AutoLocalMemory
	{
	private:
		BufferType m_Buffer;
		
	public:
		explicit AutoLocalMemory(BufferType buffer)
			: m_Buffer(buffer)
		{
		}

		~AutoLocalMemory()
		{
			LocalFree( m_Buffer );
		}
	};
}

namespace Earlgrey
{
	_txstring Log::LastErrorMessage(DWORD lastErrorCode)
	{
		LPTSTR lpMsgBuf = NULL;
		AutoLocalMemory<LPTSTR> bufferManagement(lpMsgBuf);

		DWORD retValue = FormatMessage (
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS
			, NULL
			, lastErrorCode
			, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT) // Default language
			, (LPTSTR) &lpMsgBuf
			, NULL
			, NULL
			);

		//! \todo 반환값 처리하기
		EARLGREY_VERIFY(retValue != 0);

		_txstring msg(lpMsgBuf);

		// LocalFree( lpMsgBuf );

		return msg;
	}
}