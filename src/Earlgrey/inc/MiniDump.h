#pragma once 

#ifndef _WINDOWS_
#define NOMINMAX
#include <windows.h>
#endif

#include "StlCustom.h"
#include "UnhandledExceptionHandler.h"

#include <dbghelp.h>
#include <vector>

namespace Earlgrey
{
	class MiniDump : public UnhandledExceptionHandler
	{
	public:
		class ExtendedMessage // : private Uncopyable
		{
		private:
			MINIDUMP_STREAM_TYPE m_MsgType;
			_txstring m_Msg;

		public:
			explicit ExtendedMessage(const MINIDUMP_STREAM_TYPE msgType, const _txstring msg)
				: m_MsgType(msgType)
				, m_Msg(msg)
			{
				EARLGREY_ASSERT(msgType > LastReservedStream);
			}

			ExtendedMessage(const ExtendedMessage& exMsg)
				: m_MsgType(exMsg.m_MsgType)
				, m_Msg(exMsg.m_Msg)
			{
			}

			inline MINIDUMP_STREAM_TYPE MsgType() const 
			{
				return m_MsgType;
			}

			inline const _txstring Msg() const
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
		// explicit MiniDump(const TCHAR * const dumpFilePath, size_t pathCount, MINIDUMP_TYPE dumpType);
		explicit MiniDump(MINIDUMP_TYPE dumpType);
		explicit MiniDump(const _txstring& dumpFilePath, MINIDUMP_TYPE dumpType);
		~MiniDump();
		
		void AddExtendedMessage(MINIDUMP_STREAM_TYPE msgType, _txstring msg);
		virtual void HandleException(LPEXCEPTION_POINTERS exceptionPtr);

	private: // private methods
		void Initialize();

	};
}