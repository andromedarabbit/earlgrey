#include "stdafx.h"
#include "MiniDump.h"

#include "numeric_cast.hpp"
#include "RAII.h"
#include "BasicBuffer.hpp"
#include "DateTime.h"
#include "Log.h" 

#include "txsstream.h"

using namespace std;
using namespace std::tr1;	

namespace Earlgrey
{
	MiniDump::MiniDump(MINIDUMP_TYPE dumpType)
		: m_DumpType(dumpType)
		, m_DumpFilePath()
		, m_MaxMsgLen(0)
	{
		Initialize();
	}


	MiniDump::MiniDump(const _txstring& dumpFilePath, MINIDUMP_TYPE dumpType)
		: m_DumpType(dumpType)
		, m_DumpFilePath(dumpFilePath)
		, m_MaxMsgLen(0)
	{
		EARLGREY_ASSERT(dumpFilePath.length() > 0);
		Initialize();
	}

	MiniDump::~MiniDump()
	{
		std::vector<ExtendedMessage*>::iterator it = m_ExtendedMessages.begin();
		for( ; it != m_ExtendedMessages.end(); it++)
		{
			delete *it;
		}
		m_ExtendedMessages.clear();
	}

	void MiniDump::Initialize()
	{
		if(m_DumpFilePath.length() == 0)
		{
			DateTime now(DateTime::Now());

			_txstringstream ss;
			ss << now.Year() << _T(".") << now.Month() << _T(".") << now.Day()
				<< _T(" ") << now.Hour() << now.Minute() << now.Second();

			m_DumpFilePath = ss.str();
		}

		// Custom information


	}

	void MiniDump::AddExtendedMessage(MINIDUMP_STREAM_TYPE msgType, const _txstring& msg)
	{
		m_MaxMsgLen = std::max(m_MaxMsgLen, msg.length());

		ExtendedMessage * exMsg = new ExtendedMessage(msgType, msg);
		m_ExtendedMessages.push_back(exMsg);
	}

	void MiniDump::HandleException(LPEXCEPTION_POINTERS exceptionPtrs)
	{
		EARLGREY_ASSERT(exceptionPtrs != NULL);
		
		// Open a dump file
		handle_t fileHandle(
			CreateFile(m_DumpFilePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)
			, &CloseHandle
			);
		EARLGREY_VERIFY(fileHandle.get() != INVALID_HANDLE_VALUE);


		
		// Set dump options
		MINIDUMP_EXCEPTION_INFORMATION dumpExceptionInfo;
		dumpExceptionInfo.ThreadId = GetCurrentThreadId(); 
		dumpExceptionInfo.ExceptionPointers	= exceptionPtrs;
		// The memory location to which the value of ExceptionPointers refers. 
		// If this member is TRUE, the exception pointer is located in the address space of the client, or the process that crashed. 
		// If this member is FALSE, the exception pointer is located in the address space of the calling program.
		dumpExceptionInfo.ClientPointers = true;


		// Create user information
		const std::vector<_txstring>::size_type numberOfExMessages 
			= m_ExtendedMessages.size();


		MINIDUMP_USER_STREAM* extendedMesssages = new MINIDUMP_USER_STREAM[numberOfExMessages];

		shared_ptr<MINIDUMP_USER_STREAM> autoExtendedMesssages(
			extendedMesssages
			, ArrayDeleter<MINIDUMP_USER_STREAM>() 
			);


		basic_buffer<TCHAR> buffer(m_MaxMsgLen + 1);
	
		vector<ExtendedMessage*>::iterator it = m_ExtendedMessages.begin();
		for( ; it != m_ExtendedMessages.end(); it++ )
		{
			ExtendedMessage* msg = *it;


			buffer.set(msg->Msg().c_str(), msg->Msg().length());

			extendedMesssages[0].Type        = msg->MsgType();
			extendedMesssages[0].Buffer      = buffer.data();
			extendedMesssages[0].BufferSize  = static_cast<ULONG>( msg->Msg().length() * sizeof(TCHAR) );
		}

		MINIDUMP_USER_STREAM_INFORMATION userInformation;
		// userInformation.UserStreamCount = Math::numeric_cast<ULONG>(numberOfExMessages);
		userInformation.UserStreamCount = static_cast<ULONG>(numberOfExMessages);
		userInformation.UserStreamArray = extendedMesssages;


		// Dump exception information into the file
		BOOL dumpSuccessfuly = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), fileHandle.get(), m_DumpType , &dumpExceptionInfo, NULL, NULL );
		EARLGREY_VERIFY(dumpSuccessfuly);
	}

	_txstring MiniDump::GetFaultReason(PEXCEPTION_POINTERS exPtrs)
	{
		if (::IsBadReadPtr(exPtrs, sizeof(EXCEPTION_POINTERS))) 
			return _T("bad exception pointers");

		// 간단한 에러 코드라면 그냥 변환할 수 있다.
		switch (exPtrs->ExceptionRecord->ExceptionCode)
		{
		case EXCEPTION_ACCESS_VIOLATION:         return EARLGREY_ENUM_TO_STRING("EXCEPTION_ACCESS_VIOLATION");
		case EXCEPTION_DATATYPE_MISALIGNMENT:    return EARLGREY_ENUM_TO_STRING("EXCEPTION_DATATYPE_MISALIGNMENT");
		case EXCEPTION_BREAKPOINT:               return EARLGREY_ENUM_TO_STRING("EXCEPTION_BREAKPOINT");
		case EXCEPTION_SINGLE_STEP:              return EARLGREY_ENUM_TO_STRING("EXCEPTION_SINGLE_STEP");
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return EARLGREY_ENUM_TO_STRING("EXCEPTION_ARRAY_BOUNDS_EXCEEDED");
		case EXCEPTION_FLT_DENORMAL_OPERAND:     return EARLGREY_ENUM_TO_STRING("EXCEPTION_FLT_DENORMAL_OPERAND");
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return EARLGREY_ENUM_TO_STRING("EXCEPTION_FLT_DIVIDE_BY_ZERO");
		case EXCEPTION_FLT_INEXACT_RESULT:       return EARLGREY_ENUM_TO_STRING("EXCEPTION_FLT_INEXACT_RESULT");
		case EXCEPTION_FLT_INVALID_OPERATION:    return EARLGREY_ENUM_TO_STRING("EXCEPTION_FLT_INVALID_OPERATION");
		case EXCEPTION_FLT_OVERFLOW:             return EARLGREY_ENUM_TO_STRING("EXCEPTION_FLT_OVERFLOW");
		case EXCEPTION_FLT_STACK_CHECK:          return EARLGREY_ENUM_TO_STRING("EXCEPTION_FLT_STACK_CHECK");
		case EXCEPTION_FLT_UNDERFLOW:            return EARLGREY_ENUM_TO_STRING("EXCEPTION_FLT_UNDERFLOW");
		case EXCEPTION_INT_DIVIDE_BY_ZERO:       return EARLGREY_ENUM_TO_STRING("EXCEPTION_INT_DIVIDE_BY_ZERO");
		case EXCEPTION_INT_OVERFLOW:             return EARLGREY_ENUM_TO_STRING("EXCEPTION_INT_OVERFLOW");
		case EXCEPTION_PRIV_INSTRUCTION:         return EARLGREY_ENUM_TO_STRING("EXCEPTION_PRIV_INSTRUCTION");
		case EXCEPTION_IN_PAGE_ERROR:            return EARLGREY_ENUM_TO_STRING("EXCEPTION_IN_PAGE_ERROR");
		case EXCEPTION_ILLEGAL_INSTRUCTION:      return EARLGREY_ENUM_TO_STRING("EXCEPTION_ILLEGAL_INSTRUCTION");
		case EXCEPTION_NONCONTINUABLE_EXCEPTION: return EARLGREY_ENUM_TO_STRING("EXCEPTION_NONCONTINUABLE_EXCEPTION");
		case EXCEPTION_STACK_OVERFLOW:           return EARLGREY_ENUM_TO_STRING("EXCEPTION_STACK_OVERFLOW");
		case EXCEPTION_INVALID_DISPOSITION:      return EARLGREY_ENUM_TO_STRING("EXCEPTION_INVALID_DISPOSITION");
		case EXCEPTION_GUARD_PAGE:               return EARLGREY_ENUM_TO_STRING("EXCEPTION_GUARD_PAGE");
		case EXCEPTION_INVALID_HANDLE:           return EARLGREY_ENUM_TO_STRING("EXCEPTION_INVALID_HANDLE");
			//case EXCEPTION_POSSIBLE_DEADLOCK:        return EARLGREY_ENUM_TO_STRING("EXCEPTION_POSSIBLE_DEADLOCK");
		case CONTROL_C_EXIT:                     return EARLGREY_ENUM_TO_STRING("CONTROL_C_EXIT");
		case 0xE06D7363:                         return _T("Microsoft C++ Exception");
		default:
			break;
		}

		// 뭔가 좀 더 복잡한 에러라면...
		HMODULE moduleHandle = ::GetModuleHandle(_T("ntdll.dll"));
		return Log::ErrorMessage(exPtrs->ExceptionRecord->ExceptionCode, moduleHandle);
		
	}
}