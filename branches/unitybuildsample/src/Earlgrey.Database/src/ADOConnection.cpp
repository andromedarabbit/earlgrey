#include "stdafx.h"
#include "ADOConnection.h"
#include "ADOConnectionPool.h"
#include "ADOCommand.h"
#include "ADOTransaction.h"
#include "ADOLog.h"
#include "ADOEnvironment.h"
#include "EarlgreyAssert.h"
#include "numeric_cast.hpp"

namespace Earlgrey {
namespace ADO {

	const TimeSpan Connection::DEFAULT_CONNECTION_TIMEOUT = TimeSpan(0, 0, 15);

	BOOL Connection::Open(const _txstring& connectionString, TimeSpan connectionTimeout)
	{
		return Open(connectionString.c_str(), connectionTimeout);
	}

	BOOL Connection::Open(const TCHAR* const connectionString, TimeSpan connectionTimeout)
	{
		Environment::InitializeADO();
		try
		{
			if(m_Connection == NULL)
				TESTHR(m_Connection.CreateInstance(__uuidof(RawADO::Connection)));

			m_Connection->ConnectionTimeout = EARLGREY_NUMERIC_CAST<LONG>(connectionTimeout.TotalSeconds());

			if(IsConnected() == FALSE)
				m_Connection->Open(connectionString, "", "", RawADO::adConnectUnspecified | RawADO::adUseClient);

			ConnectionPoolSingleton::Instance().BeginRegister(connectionString, NULL);
			
			return TRUE;
		}
		catch(_com_error& e)
		{
			// TODO: 임시코드
			_txstring msg( Log::FromSqlError(m_Connection, e, TRUE) );
			DBG_UNREFERENCED_LOCAL_VARIABLE(msg);
			// throw std::exception( String::FromUnicode(msg) );
		}
		return FALSE;
	} 

	BOOL Connection::IsConnected() const 
	{
		if(m_Connection == NULL)
			return FALSE;

		return (m_Connection->State != RawADO::adStateClosed)
			&& (m_Connection->State != RawADO::adStateConnecting);
	}

	void Connection::Close()
	{
		if(m_Connection == NULL)
			return;

		try
		{
			if (m_Connection->State == RawADO::adStateOpen)
				m_Connection->Close();
			m_Connection = NULL;
		}
		catch (_com_error &e)
		{
			// TODO: 임시코드
			_txstring msg( Log::FromSqlError(m_Connection, e, TRUE) );
			DBG_UNREFERENCED_LOCAL_VARIABLE(msg);
			// throw std::exception( String::FromUnicode(msg) );
		}
	}

	BOOL Connection::Execute(const TCHAR* const sql)
	{
		return Execute (sql, RawADO::adCmdStoredProc);
	}


	BOOL Connection::Execute(const TCHAR* const sql, long options)
	{
		try
		{
			m_Connection->Execute(sql, NULL, options);
			return TRUE;
		}
		catch (_com_error &e)
		{
			// TODO: 임시코드
			_txstring msg( Log::FromSqlError(m_Connection, e, TRUE) );
			DBG_UNREFERENCED_LOCAL_VARIABLE(msg);
			// throw std::exception( String::FromUnicode(msg) );
		}
		return FALSE;
	}

	void Connection::ConnectionTimeout(LONG timeout)
	{
		EARLGREY_ASSERT(m_Connection);
		m_Connection->ConnectionTimeout = timeout;
	}

	TimeSpan Connection::ConnectionTimeout()
	{
		EARLGREY_ASSERT(m_Connection);
		return TimeSpan::FromSeconds(m_Connection->ConnectionTimeout);
	}

	void Connection::CommandTimeout(LONG timeout)
	{
		EARLGREY_ASSERT(m_Connection);
		m_Connection->CommandTimeout = timeout;
	}

	LONG Connection::CommandTimeout()
	{
		EARLGREY_ASSERT(m_Connection);
		return m_Connection->CommandTimeout;
	}

	Command Connection::CreateCommand()
	{
		return Command(_T(""), *this);
	}

	std::tr1::shared_ptr<Transaction> Connection::BeginTransaction()
	{
		return std::tr1::shared_ptr<Transaction>(
			new Transaction(*this)
			);
	}



}
}