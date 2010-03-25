#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"
#include "ADO.h"
#include "ADOCast.h"

namespace Earlgrey
{
	namespace ADO
	{

		class DataReader // : private Earlgrey::Uncopyable
		{
			friend class Command;

			explicit DataReader(RawADO::_RecordsetPtr recordset)
				: m_Recordset(recordset)
				, m_Index(static_cast<SHORT>(0))
				, m_HasReadFirstRecord(FALSE)
			{
				EARLGREY_ASSERT(m_Recordset != NULL);
			}


		public:	
			~DataReader()
			{
				Close();
			}

			inline BOOL Read()
			{
				EARLGREY_ASSERT(m_Recordset != NULL);
				
				if(m_HasReadFirstRecord == TRUE) 
				{
					m_Index = static_cast<SHORT>(0);
					m_Recordset->MoveNext();
				}
				m_HasReadFirstRecord = TRUE;
				return m_Recordset->EndOfFile == false && m_Recordset->BOF == false;
			}

			inline BOOL NextResult() 
			{
				EARLGREY_ASSERT(m_Recordset != NULL);

				long recordsAffected = 0;
				m_Recordset = m_Recordset->NextRecordset((VARIANT *)&recordsAffected);
				return m_Recordset != NULL;
			}

			inline void Close()
			{
				if (m_Recordset)
					if (m_Recordset->State == RawADO::adStateOpen)
						m_Recordset->Close();
			}

			template <typename T>
			inline DataReader& operator>>(T& x)
			{
				x = GetValue<T>(m_Index);
				m_Index++;
				return *this;
			}

			template <typename T>
			inline T GetValue(SHORT i) const
			{
				const _variant_t index(i, VT_I2);
				const _variant_t value(
					m_Recordset->Fields->GetItem(&index)->GetValue()
					);

				return database_cast<T>(value);
			}

		private:
			RawADO::_RecordsetPtr m_Recordset;
			SHORT m_Index;
			BOOL m_HasReadFirstRecord;
		};
	}
}