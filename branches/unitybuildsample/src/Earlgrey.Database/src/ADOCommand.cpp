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

	std::tr1::shared_ptr<DataReader> Command::ExecuteReader()
	{
		try
		{
			// TODO: 임시 코드
			// _variant_t recordsAffected(0L);
			// RawADO::_RecordsetPtr recordset = m_Command->Execute(&recordsAffected, NULL, NULL);
			RawADO::_RecordsetPtr recordset = m_Command->Execute(NULL, NULL, NULL);
			/*std::tr1::shared_ptr<RawADO::_Recordset> recordset(
			m_Command->Execute(NULL, NULL, NULL)
			, Deleter<RawADO::_RecordsetPtr>()
			);*/

			//return DataReader(recordset);
			return std::tr1::shared_ptr<DataReader>(
				new DataReader(recordset)
				);
		}
		catch(_com_error& e)
		{
			const _txstring msg( Log::FromSqlError(m_Command, e, TRUE) );
			throw std::exception( String::FromUnicode(msg) );
		}
	}

	LONG Command::ExecuteNonQuery(BOOL withRecordsAffected)
	{
		long options = RawADO::adCmdUnspecified;
		if(withRecordsAffected == FALSE)
			options = RawADO::adExecuteNoRecords;

		try
		{
			const LONG recordsAffected = m_Command->Execute(NULL, NULL, options);
			return recordsAffected;
		}
		catch(_com_error& e)
		{
			const _txstring msg( Log::FromSqlError(m_Command, e, TRUE) );
			throw std::exception( String::FromUnicode(msg) );
		}
	}



	
}
}