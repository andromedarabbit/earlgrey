#pragma once 
#include "EarlgreyAssert.h"
#include "txstring.h"
#include "UnhandledExceptionHandler.h"

#include <dbghelp.h>
#include <vector>

#pragma comment(lib, "Dbghelp.lib")

namespace Earlgrey
{
	//! \ref http://serious-code.net/moin.cgi/MiniDump
	class MiniDump : public UnhandledExceptionHandler
	{
	public:
		class ExtendedMessage // : private Uncopyable
		{
		private:
			MINIDUMP_STREAM_TYPE m_MsgType;
			xwstring m_Msg;

		public:
			explicit ExtendedMessage(const MINIDUMP_STREAM_TYPE msgType, const xwstring& msg)
				: m_MsgType(msgType)
				, m_Msg(msg)
			{
				EARLGREY_ASSERT(msgType > LastReservedStream);
			}

			explicit ExtendedMessage(const ExtendedMessage& exMsg)
				: m_MsgType(exMsg.m_MsgType)
				, m_Msg(exMsg.m_Msg)
			{
			}

			inline MINIDUMP_STREAM_TYPE MsgType() const 
			{
				return m_MsgType;
			}

			inline const xwstring& Msg() const
			{
				return m_Msg;
			}		
		};
		
	private: 
		xwstring m_DumpFilePath;
		std::vector<ExtendedMessage*> m_ExtendedMessages;
		xwstring::size_type m_MaxMsgLen;

		MINIDUMP_TYPE m_DumpType;

	public:
		explicit MiniDump(const xwstring& dumpFilePath, MINIDUMP_TYPE dumpType);
		virtual ~MiniDump();

		virtual const WCHAR * ClassName() const;

		inline const xwstring& FilePath() const
		{
			return m_DumpFilePath;
		}

		void AddExtendedMessage(MINIDUMP_STREAM_TYPE msgType, const xwstring& msg);
		virtual void HandleException(LPEXCEPTION_POINTERS exceptionPtrs);

		static const WCHAR * const GetFaultReason(PEXCEPTION_POINTERS exPtrs);
	private: // private methods
		void Initialize();

	};
}