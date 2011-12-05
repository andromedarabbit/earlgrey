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
			_txstring m_Msg;

		public:
			explicit ExtendedMessage(const MINIDUMP_STREAM_TYPE msgType, const _txstring& msg)
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

			inline const _txstring& Msg() const
			{
				return m_Msg;
			}		
		};
		
	private: 
		_txstring m_DumpFilePath;
		std::vector<ExtendedMessage*> m_ExtendedMessages;
		_txstring::size_type m_MaxMsgLen;

		MINIDUMP_TYPE m_DumpType;

	public:
		explicit MiniDump(const _txstring& dumpFilePath, MINIDUMP_TYPE dumpType);
		virtual ~MiniDump();

		virtual const TCHAR * ClassName() const;

		inline const _txstring& FilePath() const
		{
			return m_DumpFilePath;
		}

		void AddExtendedMessage(MINIDUMP_STREAM_TYPE msgType, const _txstring& msg);
		virtual void HandleException(LPEXCEPTION_POINTERS exceptionPtrs);

		static const TCHAR * const GetFaultReason(PEXCEPTION_POINTERS exPtrs);
	private: // private methods
		void Initialize();

	};
}