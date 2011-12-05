#include "stdafx.h"
#include "ADOLog.h"

#include "txstring.h"

namespace Earlgrey
{
	namespace ADO
	{
		_txstring Log::FromSqlError(RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors)
		{
			_txostringstream ss;
			FromSqlError(ss, command, e, clearErrors);
			return _txstring(ss.str());
		}

		void Log::FromSqlError(_txostringstream& ss, RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors)
		{
			if(command->CommandType == RawADO::adCmdStoredProc)
				ss << TEXT("Stored Procedure ");
			else
				ss << TEXT("Query ");

			ss << _T("'") << command->CommandText << _T("' failed!") << std::endl;

			FromSqlError(ss, command->ActiveConnection, e, clearErrors);
		}

		_txstring Log::FromSqlError(RawADO::_ConnectionPtr pConnection, const _com_error &e, BOOL clearErrors)
		{
			_txostringstream ss;
			FromSqlError(ss, pConnection, e, clearErrors);
			return _txstring(ss.str());
		}

		void Log::FromSqlError(_txostringstream& ss, RawADO::_ConnectionPtr pConnection, const _com_error &e, BOOL clearErrors)
		{
			FromProviderError(ss, pConnection, clearErrors);
			FromComError(ss, e);
		}

		_txstring Log::FromProviderError(RawADO::_ConnectionPtr pConnection, BOOL clearErrors)
		{
			_txostringstream ss;
			FromProviderError(ss, pConnection, clearErrors);
			return _txstring(ss.str());
		}

		void Log::FromProviderError(_txostringstream& ss, RawADO::_ConnectionPtr pConnection, BOOL clearErrors)
		{
			if(pConnection == NULL)
				return;

			// Print Provider Errors from Connection object.
			// pErr is a record object in the Connection's Error collection.
			RawADO::ErrorPtr  pErr = NULL;

			if( (pConnection->Errors->Count) > 0)
			{
				long nCount = pConnection->Errors->Count;

				// Collection ranges from 0 to nCount -1.
				for(long i = 0; i < nCount; i++)
				{
					pErr = pConnection->Errors->GetItem(i);
					FromError(ss, pErr);
				}
			}

			if(clearErrors)
				pConnection->Errors->Clear();
		}

		_txstring Log::FromComError(const _com_error &e)
		{
			_txostringstream ss;
			FromComError(ss, e);
			return _txstring(ss.str());
		}

		void Log::FromComError(_txostringstream& ss, const _com_error &e)
		{
			ss << _T("SQL ComError Code: ") << e.Error()
				<< _T(", Meaning: '") << e.ErrorMessage() << _T("'")
				;


			_bstr_t bstrSource(e.Source());
			const TCHAR* const source = (LPCTSTR)bstrSource; 

			if(source != NULL)
				ss << _T(", Source: '") << source << _T("'");


			_bstr_t bstrDescription(e.Description());
			const TCHAR* const description = (LPCTSTR)bstrDescription;

			if(description != NULL)
				ss << _T(", Desc: '") << source << _T("'");

			ss << std::endl;
		}


		_txstring Log::FromError(RawADO::ErrorPtr error)
		{
			_txostringstream ss;
			FromError(ss, error);
			return _txstring(ss.str());
		}

		void Log::FromError(_txostringstream& ss, RawADO::ErrorPtr error)
		{
			if(error)
				ss << _T("SQL Error Number: " << error->Number << _T(", Desc: ") << (LPCTSTR)error->Description ) << std::endl;
		}

	}
}