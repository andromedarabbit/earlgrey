#pragma once
#include "Uncopyable.h"
#include "txstring.h"
#include "ADO.h"

#include "ADOCast.h"
#include "ADODataReader.h"
#include "ADOLog.h"

namespace Earlgrey {

	namespace ADO {

		class Connection;

		class Command : private Uncopyable
		{
		public:
			explicit Command();
			explicit Command(const _txstring& cmdText);
			explicit Command(const _txstring& cmdText, Connection& connection);

			inline const TCHAR * CommandText() const
			{
				EARLGREY_ASSERT(m_Command != NULL);
				return m_Command->CommandText;
			}

			inline void CommandText(const _txstring& cmdText)
			{
				return CommandText(cmdText.c_str());
			}

			inline void CommandText(const TCHAR * cmdText)
			{
				EARLGREY_ASSERT(m_Command != NULL);
				m_Command->CommandText = cmdText;
			}

			inline LONG CommandTimeout() const 
			{
				EARLGREY_ASSERT(m_Command != NULL);
				return m_Command->CommandTimeout;
			}

			inline void CommandTimeout(LONG commandTimeout) const 
			{
				EARLGREY_ASSERT(m_Command != NULL);
				m_Command->CommandTimeout = commandTimeout;
			}

			inline void Close()
			{
				if(m_Command != NULL)
					if (m_Command->State == RawADO::adStateOpen)
						m_Command->Release();
			}

			inline Earlgrey::CommandType CommandType() const
			{
				return database_cast<Earlgrey::CommandType>(m_Command->CommandType);
			}

			inline void CommandType(Earlgrey::CommandType cmdType) const
			{
				m_Command->CommandType = database_cast<RawADO::CommandTypeEnum>(cmdType);
			}

			std::tr1::shared_ptr<DataReader> ExecuteReader();

			template<typename T>
			inline T ExecuteScalar()
			{	
				static const _variant_t index(static_cast<SHORT>(0), VT_I2);

				// TODO: adExecuteRecord를 어떻게 쓰는지 몰라서 우선 이렇게 처리한다.
				// 					RawADO::_RecordPtr record = m_Command->Execute(NULL, NULL, RawADO::adExecuteRecord);
				// 					const _variant_t value(
				// 						record->Fields->GetItem(&index)->GetValue()
				// 						);
				// 					record->Close();
				// 
				// 					return database_cast<T>(value);
				// 					
				// DataReader reader(ExecuteReader());
				std::tr1::shared_ptr<DataReader> reader(ExecuteReader());
				if(reader->Read() == FALSE)
					throw std::exception("");
				return reader->GetValue<T>(0);
			}	

			LONG ExecuteNonQuery(BOOL withRecordsAffected = FALSE);

		private:
			RawADO::_CommandPtr m_Command;
		};

	}
}