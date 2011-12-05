#pragma once
#include "Uncopyable.h"
#include "ADO.h"
#include "TimeSpan.h"

namespace Earlgrey {
namespace ADO {


	class Command;
	class Transaction;

	class Connection 
		: private Uncopyable
	{
		friend class Command;
		friend class Transaction;

	public:
		const static TimeSpan DEFAULT_CONNECTION_TIMEOUT;

		explicit Connection()
			: m_Connection(NULL)
		{
		}

		~Connection()
		{
			Close();
		}


		BOOL Open(const _txstring& connectionString, TimeSpan connectionTimeout = DEFAULT_CONNECTION_TIMEOUT);
		BOOL Open(const TCHAR* const connectionString, TimeSpan connectionTimeout = DEFAULT_CONNECTION_TIMEOUT);
		BOOL IsConnected() const;

		void Close();

		/*inline RawADO::_ConnectionPtr GetRawConnection() const
		{
			return m_Connection;
		}*/

		BOOL Execute(const TCHAR* const sql);
		BOOL Execute(const TCHAR* const sql, long options);

		void ConnectionTimeout(LONG timeout);
		TimeSpan ConnectionTimeout();
		void CommandTimeout(LONG timeout);
		LONG CommandTimeout();

		Command CreateCommand();

		std::tr1::shared_ptr<Transaction> BeginTransaction();

	private: // methods
		inline RawADO::_ConnectionPtr GetRawConnection() const
		{
			return m_Connection;
		}

	private:
		RawADO::_ConnectionPtr m_Connection;

	};

	
}
}