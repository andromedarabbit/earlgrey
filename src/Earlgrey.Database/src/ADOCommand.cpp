#include "stdafx.h"
#include "ADOCommand.h"

#include "ADOConnection.h"
#include "ADOLog.h"

namespace Earlgrey {
namespace ADO {

	Command::Command()
		: m_Command(NULL)
	{
	}

 	Command::Command(const _txstring& cmdText)
		: m_Command(NULL)
	{
		TESTHR(m_Command.CreateInstance(__uuidof(RawADO::Command)));
		m_Command->CommandText = cmdText.c_str();
	}

	Command::Command(const _txstring& cmdText, Connection& connection)
		: m_Command(NULL)
	{
		TESTHR(m_Command.CreateInstance(__uuidof(RawADO::Command)));
		m_Command->CommandText = cmdText.c_str();
		m_Command->ActiveConnection = connection.GetRawConnection();
	}


	
}
}