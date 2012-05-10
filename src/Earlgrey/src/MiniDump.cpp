#include "stdafx.h"
#include "MiniDump.h"

#include "numeric_cast.hpp"
#include "RAII.h"
#include "BasicBuffer.hpp"
#include "DateTime.h"
#include "Log.h" 

#include "txsstream.h"
#include "RAII.h"
#include "Macros.h"


namespace Earlgrey
{
	MiniDump::MiniDump(const xwstring& dumpFilePath, MINIDUMP_TYPE dumpType)
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

			xwstringstream ss;
			ss << now.Year() << L"." << now.Month() << L"." << now.Day()
				<< L" " << now.Hour() << now.Minute() << now.Second();

			m_DumpFilePath = ss.str();
		}

		// Custom information


	}

	const WCHAR * MiniDump::ClassName() const
	{
		return L"Earlgrey::MiniDump";
	}

	void MiniDump::AddExtendedMessage(MINIDUMP_STREAM_TYPE msgType, const xwstring& msg)
	{
		m_MaxMsgLen = std::max(m_MaxMsgLen, msg.length());

		ExtendedMessage * exMsg = new ExtendedMessage(msgType, msg);
		m_ExtendedMessages.push_back(exMsg);
	}

	void MiniDump::HandleException(LPEXCEPTION_POINTERS exceptionPtrs)
	{
		using namespace std;
		using namespace std::tr1;	

		EARLGREY_ASSERT(exceptionPtrs != NULL);
		
		// Open a dump file
		handle_t fileHandle(
			CreateFileW(m_DumpFilePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)
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
		const std::vector<xwstring>::size_type numberOfExMessages 
			= m_ExtendedMessages.size();


		MINIDUMP_USER_STREAM* extendedMesssages = new MINIDUMP_USER_STREAM[numberOfExMessages];

		shared_ptr<MINIDUMP_USER_STREAM> autoExtendedMesssages(
			extendedMesssages
			, ArrayDeleter<MINIDUMP_USER_STREAM>() 
			);


		basic_buffer<WCHAR> buffer(m_MaxMsgLen + 1);
	
		vector<ExtendedMessage*>::iterator it = m_ExtendedMessages.begin();
		for( ; it != m_ExtendedMessages.end(); it++ )
		{
			ExtendedMessage* msg = *it;


			buffer.set(msg->Msg().c_str(), msg->Msg().length());

			extendedMesssages[0].Type        = msg->MsgType();
			extendedMesssages[0].Buffer      = buffer.data();
			extendedMesssages[0].BufferSize  = static_cast<ULONG>( msg->Msg().length() * sizeof(WCHAR) );
		}

		MINIDUMP_USER_STREAM_INFORMATION userInformation;
		// userInformation.UserStreamCount = EARLGREY_NUMERIC_CAST<ULONG>(numberOfExMessages);
		userInformation.UserStreamCount = static_cast<ULONG>(numberOfExMessages);
		userInformation.UserStreamArray = extendedMesssages;


		// Dump exception information into the file
		BOOL dumpSuccessfuly = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), fileHandle.get(), m_DumpType , &dumpExceptionInfo, NULL, NULL );
		EARLGREY_VERIFY(dumpSuccessfuly);
	}

	const WCHAR * const MiniDump::GetFaultReason(PEXCEPTION_POINTERS exPtrs)
	{
		if (::IsBadReadPtr(exPtrs, sizeof(EXCEPTION_POINTERS))) 
			return L"bad exception pointers";

		// 간단한 에러 코드라면 그냥 변환할 수 있다.
		switch (exPtrs->ExceptionRecord->ExceptionCode)
		{
		case EXCEPTION_ACCESS_VIOLATION:         return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_ACCESS_VIOLATION");
		case EXCEPTION_DATATYPE_MISALIGNMENT:    return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_DATATYPE_MISALIGNMENT");
		case EXCEPTION_BREAKPOINT:               return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_BREAKPOINT");
		case EXCEPTION_SINGLE_STEP:              return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_SINGLE_STEP");
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_ARRAY_BOUNDS_EXCEEDED");
		case EXCEPTION_FLT_DENORMAL_OPERAND:     return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_FLT_DENORMAL_OPERAND");
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_FLT_DIVIDE_BY_ZERO");
		case EXCEPTION_FLT_INEXACT_RESULT:       return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_FLT_INEXACT_RESULT");
		case EXCEPTION_FLT_INVALID_OPERATION:    return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_FLT_INVALID_OPERATION");
		case EXCEPTION_FLT_OVERFLOW:             return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_FLT_OVERFLOW");
		case EXCEPTION_FLT_STACK_CHECK:          return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_FLT_STACK_CHECK");
		case EXCEPTION_FLT_UNDERFLOW:            return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_FLT_UNDERFLOW");
		case EXCEPTION_INT_DIVIDE_BY_ZERO:       return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_INT_DIVIDE_BY_ZERO");
		case EXCEPTION_INT_OVERFLOW:             return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_INT_OVERFLOW");
		case EXCEPTION_PRIV_INSTRUCTION:         return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_PRIV_INSTRUCTION");
		case EXCEPTION_IN_PAGE_ERROR:            return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_IN_PAGE_ERROR");
		case EXCEPTION_ILLEGAL_INSTRUCTION:      return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_ILLEGAL_INSTRUCTION");
		case EXCEPTION_NONCONTINUABLE_EXCEPTION: return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_NONCONTINUABLE_EXCEPTION");
		case EXCEPTION_STACK_OVERFLOW:           return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_STACK_OVERFLOW");
		case EXCEPTION_INVALID_DISPOSITION:      return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_INVALID_DISPOSITION");
		case EXCEPTION_GUARD_PAGE:               return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_GUARD_PAGE");
		case EXCEPTION_INVALID_HANDLE:           return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_INVALID_HANDLE");
			//case EXCEPTION_POSSIBLE_DEADLOCK:        return EARLGREY_ENUM_TO_STRING_W("EXCEPTION_POSSIBLE_DEADLOCK");
		case CONTROL_C_EXIT:                     return EARLGREY_ENUM_TO_STRING_W("CONTROL_C_EXIT");
		case 0xE06D7363:                         return L"Microsoft C++ Exception";
		default:
			break;
		}

		// 뭔가 좀 더 복잡한 에러라면...
		HMODULE moduleHandle = ::GetModuleHandleW(L"ntdll.dll");
		return Log::ErrorMessageW(exPtrs->ExceptionRecord->ExceptionCode, moduleHandle);
		
	}
}