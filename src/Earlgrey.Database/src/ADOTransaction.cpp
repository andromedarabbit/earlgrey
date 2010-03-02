#include "stdafx.h"
#include "ADOTransaction.h"
#include "ADOConnection.h"

namespace Earlgrey {
	namespace ADO {

		Transaction::Transaction(Connection& connection)
			: m_Connection(connection)
			, m_IsValid(TRUE)
		{
			// EARLGREY_ASSERT(m_Connection != NULL);
			EARLGREY_ASSERT(m_Connection.GetRawConnection() != NULL);

			m_Connection.GetRawConnection()->BeginTrans();
		}

		Transaction::~Transaction()
		{
			Close();
		}

		void Transaction::Close()
		{
			if(m_IsValid)
				Rollback();
		}

		void Transaction::Commit()
		{
			// EARLGREY_ASSERT(m_Connection != NULL);
			EARLGREY_ASSERT(m_Connection.GetRawConnection() != NULL);
			
			if(m_IsValid == FALSE)
				throw std::exception("InvalidOperationException");
			
			m_Connection.GetRawConnection()->CommitTrans();
			m_IsValid = FALSE;
		}

		void Transaction::Rollback()
		{
			// EARLGREY_ASSERT(m_Connection != NULL);
			EARLGREY_ASSERT(m_Connection.GetRawConnection() != NULL);

			if(m_IsValid == FALSE)
				throw std::exception("InvalidOperationException");
			
			m_Connection.GetRawConnection()->RollbackTrans();
			m_IsValid = FALSE;
		}
	}
}
