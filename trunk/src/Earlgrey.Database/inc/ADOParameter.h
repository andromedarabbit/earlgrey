#pragma once
#include "Uncopyable.h"
#include "ADO.h"
#include "ADOCast.h"

#include "ADOLog.h"

namespace Earlgrey {
	namespace ADO {

		class Parameter : private Uncopyable
		{
		public:
			explicit Parameter()
				: m_Parameter(NULL)
			{
				TESTHR(m_Parameter.CreateInstance(__uuidof(RawADO::Parameter)));
			}

			template<typename T>
			inline void Value(const T& value)
			{
				m_Parameter->Value = database_cast<_variant_t>(value);
			}

			template<typename T>
			inline T Value() const 
			{
				return database_cast<T>(m_Parameter->Value);
			}

			inline const TCHAR * ParameterName() const 
			{
				return database_cast<const TCHAR *>(m_Parameter->Name);
			}

			inline 
				void ParameterName(const TCHAR * parameterName)
			{
				m_Parameter->Name =  database_cast<_bstr_t>(parameterName);
			}

			inline 
				void ParameterName(const _txstring& parameterName)
			{
				return ParameterName(parameterName.c_str());
			}

			inline 
				void Direction(const ParameterDirection direction)
			{
				m_Parameter->Direction = database_cast<RawADO::ParameterDirectionEnum>(direction);
			}

			inline 
				ParameterDirection Direction() const
			{
				return database_cast<ParameterDirection>(m_Parameter->Direction);
			}

			inline 
				void Size(const long size)
			{
				m_Parameter->Size = size;
			}

			inline 
				long Size() const
			{
				return m_Parameter->Size;
			}

			inline 
				void Precision(const BYTE precision)
			{
				m_Parameter->Precision = precision;
			}

			inline 
				BYTE Precision () const
			{
				return m_Parameter->Precision;
			}

			inline 
				void Scale(const BYTE scale)
			{
				m_Parameter->NumericScale = scale;
			}

			inline 
				BYTE Scale () const
			{
				return m_Parameter->NumericScale;
			}


		private:
			RawADO::_ParameterPtr m_Parameter;
		};

	} // !ADO
} // !Earlgrey

