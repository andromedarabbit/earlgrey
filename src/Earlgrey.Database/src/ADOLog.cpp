#include "stdafx.h"
#include "ADOLog.h"

#include "txstring.h"

namespace Earlgrey
{
	namespace ADO
	{
		using namespace RawADO;

		void Log::ReportSqlError(_CommandPtr command, _com_error &e, BOOL clearErrors)
		{		
			_txstring message;
			_txstring queryMessage;
			_txstring commandTxt;
			if(command->CommandType == adCmdStoredProc)
				queryMessage = TEXT("Stored Procedure");
			else
				queryMessage = TEXT("Query");

			commandTxt = command->CommandText;

			// Message = FormatStr(TEXT("%s '%s' was Failed to Execution!"),QueryMessage.c_str(),CommandTxt.c_str());

			// Logf( LT_WARN, _T("%s"), Message.c_str() );
			ReportSqlError(command->ActiveConnection, e, clearErrors);
		}

		void Log::ReportSqlError(_ConnectionPtr pConnection, _com_error &e, BOOL clearErrors)
		{
			ReportProviderError(pConnection, clearErrors);
			ReportComError(e);
		}

		void Log::ReportProviderError(_ConnectionPtr pConnection, BOOL clearErrors)
		{
			if(pConnection == NULL)
				return;

			// Print Provider Errors from Connection object.
			// pErr is a record object in the Connection's Error collection.
			ErrorPtr  pErr = NULL;

			if( (pConnection->Errors->Count) > 0)
			{
				long nCount = pConnection->Errors->Count;

				// Collection ranges from 0 to nCount -1.
				for(long i = 0; i < nCount; i++)
				{
					pErr = pConnection->Errors->GetItem(i);
					ReportError(pErr);
				}
			}

			if(clearErrors)
				pConnection->Errors->Clear();
		}

		void Log::ReportComError(_com_error &e)
		{
 			_bstr_t bstrSource(e.Source());
// 			const TCHAR* const source = (LPCTSTR)bstrSource; 
 			_bstr_t bstrDescription(e.Description());
// 			const TCHAR* const description = (LPCTSTR)bstrDescription;

			DBG_UNREFERENCED_LOCAL_VARIABLE(bstrSource);
			DBG_UNREFERENCED_LOCAL_VARIABLE(bstrDescription);

// 			_txstring SourceMsg;
// 			_txstring DescMsg;

// 			if(source != NULL)
// 				SourceMsg = FormatStr(TEXT("Source:'%s'"), source);
// 
// 			if(description != NULL)
// 				DescMsg = FormatStr(TEXT("Desc:'%s'"), description);
// 
// 			_txstring Message = FormatStr(TEXT("SQL ComError Code:%d, Meaning:'%s', %s, %s"),
// 				e.Error(),			
// 				e.ErrorMessage(),
// 				SourceMsg.c_str(),
// 				DescMsg.c_str()
// 				);
// 
// 			Logf( LT_WARN, _T("%s"), Message.c_str() );	
		}

		// void Log::ReportError(struct Error* error)
		void Log::ReportError(ErrorPtr error)
		{
			DBG_UNREFERENCED_PARAMETER(error);
			// if(error)
				// Logf( LT_WARN, _T("SQL Error Number: %x, Desc:%s"), error->Number, (LPCTSTR)error->Description );
		}
	}
}