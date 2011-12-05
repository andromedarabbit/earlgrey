#pragma once
#include "Uncopyable.h"
#include "ADO.h"
#include "txstring.h"

namespace Earlgrey {
	namespace ADO {

		class Connection;

		class Transaction : private Uncopyable
		{
			friend class Connection;

			explicit Transaction(Connection& connection);
		
		public:
			~Transaction();

			void Close();

			void Commit();

			void Rollback();
			// void Rollback(const _txstring& transactionName);

			// void Save(const _txstring& transactionName);

		private: 
			Connection& m_Connection;
			BOOL m_IsValid;
		};
	}
}
 