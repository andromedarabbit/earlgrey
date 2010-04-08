#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"
#include "ADO.h"
#include "ADOCast.h"
#include "ADOLog.h"

#include <ios>


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
				, m_State(std::ios::goodbit)
			{
				EARLGREY_ASSERT(m_Recordset != NULL);
			}


		public:	
			~DataReader()
			{
				Close();
			}

			BOOL Read();

			inline 
				BOOL NextResult() 
			{
				EARLGREY_ASSERT(m_Recordset != NULL);

				long recordsAffected = 0;
				m_Recordset = m_Recordset->NextRecordset((VARIANT *)&recordsAffected);
				return m_Recordset != NULL;
			}

			inline 
				void Close()
			{
				if (m_Recordset)
					if (m_Recordset->State == RawADO::adStateOpen)
						m_Recordset->Close();
			}

			template <typename T>
			inline DataReader& operator >> (T& x)
			{
				if(fail())
					return *this;

				try
				{
					x = GetValue<T>(m_Index);
					m_Index++;
					return *this;
				}
				catch(std::exception& e)
				{
					// TODO 임시 코드
					DBG_UNREFERENCED_PARAMETER(e);
					setstate(std::ios::failbit);
					return *this;
				}
			}

			template <typename T>
			inline T GetValue(SHORT i) // const
			{
				const _variant_t index(i, VT_I2);
 				try
 				{
					const _variant_t value(
						m_Recordset->Fields->GetItem(&index)->GetValue()
						);

					return database_cast<T>(value);
 				}
				catch(_com_error& e)
				{
					// FIXME: 임시 코드
					const _txstring msg( Log::FromComError(e) );
					throw std::exception( String::FromUnicode(msg) );
				}		
			}

			inline 
				std::ios::iostate rdstate() const 
			{
				return m_State;
			}

			inline 
				void clear(std::ios::iostate state = std::ios::goodbit)
			{
				m_State = state;
			}

			inline
				void setstate(std::ios::io_state state)
			{
				clear ( rdstate() | state );
			}

			inline
				bool good() const
			{	// test if no state bits are set
				return (rdstate() == std::ios::goodbit);
			}

			inline
				bool eof() const
			{	// test if eofbit is set in stream state
				return ((int)rdstate() & (int)std::ios::eofbit);
			}

			inline
				bool fail() const
			{	// test if badbit or failbit is set in stream state
				return (((int)rdstate()
					& ((int)std::ios::badbit | (int)std::ios::failbit)) != 0);
			}

			inline
				bool bad() const
			{	// test if badbit is set in stream state
				return (((int)rdstate() & (int)std::ios::badbit) != 0);
			}

		private:
			RawADO::_RecordsetPtr m_Recordset;
			SHORT m_Index;
			BOOL m_HasReadFirstRecord;
			std::ios::iostate m_State;
		};
	}
}