#include "stdafx.h"
#include "MiniDump.h"

#include "numeric_cast.hpp"
#include "RAII.h"
#include "BasicBuffer.hpp"
#include "DateTime.h"

namespace Earlgrey
{
	using namespace std;
	using namespace std::tr1;	

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

	void MiniDump::AddExtendedMessage(MINIDUMP_STREAM_TYPE msgType, _txstring msg)
	{
		m_MaxMsgLen = std::max(m_MaxMsgLen, msg.length());

		ExtendedMessage * exMsg = new ExtendedMessage(msgType, msg);
		m_ExtendedMessages.push_back(exMsg);
	}

	void MiniDump::HandleException(LPEXCEPTION_POINTERS exceptionPtr)
	{
		EARLGREY_ASSERT(exceptionPtr != NULL);
		
		// Open a dump file
		handle_t fileHandle(
			CreateFile(m_DumpFilePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)
			, &CloseHandle
			);
		EARLGREY_VERIFY(fileHandle.get() != INVALID_HANDLE_VALUE);


		
		// Set dump options
		MINIDUMP_EXCEPTION_INFORMATION dumpExceptionInfo;
		dumpExceptionInfo.ThreadId = GetCurrentThreadId(); 
		dumpExceptionInfo.ExceptionPointers	= exceptionPtr;
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
}