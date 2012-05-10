#include "stdafx.h"
#include "ADOLog.h"

#include "txstring.h"
// #include "StlCustomAllocator.hpp"

namespace Earlgrey
{
	namespace ADO
	{
		namespace
		{
			template<typename T>
			void LogFromSqlError(std::basic_ostringstream<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type >& ss, RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors)
			{
				if(command->CommandType == RawADO::adCmdStoredProc)
					ss << "Stored Procedure ";
				else
					ss << "Query ";

				ss << "'" << command->CommandText << "' failed!" << std::endl;

				Log::FromSqlError(ss, command->ActiveConnection, e, clearErrors);
			}

		}

		void Log::FromSqlError(xwostringstream& ss, RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors)
		{
			LogFromSqlError(ss, command, e, clearErrors);
		}

		void Log::FromSqlError(xostringstream& ss, RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors)
		{
			LogFromSqlError(ss, command, e, clearErrors);
		}

		xwstring Log::FromSqlErrorW(RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors)
		{
			return FromSqlError<WCHAR>(command, e, clearErrors);
		}
		xstring Log::FromSqlErrorA(RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors)
		{
			return FromSqlError<CHAR>(command, e, clearErrors);
		}



		void Log::FromSqlError(xwostringstream& ss, RawADO::_ConnectionPtr connection, const _com_error &e, BOOL clearErrors)
		{
			FromProviderError(ss, connection, clearErrors);
			FromComError(ss, e);
		}

		void Log::FromSqlError(xostringstream& ss, RawADO::_ConnectionPtr connection, const _com_error &e, BOOL clearErrors)
		{
			FromProviderError(ss, connection, clearErrors);
			FromComError(ss, e);
		}

		xwstring Log::FromSqlErrorW(RawADO::_ConnectionPtr connection, const _com_error &e, BOOL clearErrors)
		{
			return FromSqlError<WCHAR>(connection, e, clearErrors);
		}

		xstring Log::FromSqlErrorA(RawADO::_ConnectionPtr connection, const _com_error &e, BOOL clearErrors)
		{
			return FromSqlError<CHAR>(connection, e, clearErrors);
		}

		namespace 
		{
			template<typename T>		
			void LogFromProviderError(std::basic_ostringstream<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type >& ss, RawADO::_ConnectionPtr connection, BOOL clearErrors)
			{
				if(connection == NULL)
					return;

				// Print Provider Errors from Connection object.
				// pErr is a record object in the Connection's Error collection.
				RawADO::ErrorPtr  pErr = NULL;

				if( (connection->Errors->Count) > 0)
				{
					long nCount = connection->Errors->Count;

					// Collection ranges from 0 to nCount -1.
					for(long i = 0; i < nCount; i++)
					{
						pErr = connection->Errors->GetItem(i);
						Log::FromError(ss, pErr);
					}
				}

				if(clearErrors)
					connection->Errors->Clear();
			}

		}
		
		void Log::FromProviderError(xwostringstream& ss, RawADO::_ConnectionPtr connection, BOOL clearErrors)
		{
			LogFromProviderError(ss, connection, clearErrors);
		}

		void Log::FromProviderError(xostringstream& ss, RawADO::_ConnectionPtr connection, BOOL clearErrors)
		{
			LogFromProviderError(ss, connection, clearErrors);
		}

	

		namespace
		{
			template<typename T>
			void LogFromComError(
				std::basic_ostringstream<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type >& ss
				, const _com_error &e
				)
			{
				ss << "SQL ComError Code: " << e.Error()
					<< ", Meaning: '" << e.ErrorMessage() << "'"
					;


				const _bstr_t bstrSource(e.Source());
				const T* const source = (const T*)bstrSource; 

				if(source != NULL)
					ss << ", Source: '" << source << "'"
					;


				const _bstr_t bstrDescription(e.Description());
				const T* const description = (const T*)bstrDescription;

				if(description != NULL)
					ss << ", Desc: '" << source << "'"
					;

				ss << std::endl;
			}

		}

		void Log::FromComError(xwostringstream& ss, const _com_error &e)
		{
			return LogFromComError(ss, e);
		}

		void Log::FromComError(xostringstream& ss, const _com_error &e)
		{
			return LogFromComError(ss, e);
		}

		xwstring Log::FromComErrorW(const _com_error& e)
		{
			return FromComError<WCHAR>(e);
		}

		xstring Log::FromComErrorA(const _com_error& e)
		{
			return FromComError<CHAR>(e);
		}

		namespace
		{
			template<typename T>
			void LogFromError(std::basic_ostringstream<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type >& ss, RawADO::ErrorPtr error)
			{
				if(error)
					ss << _T("SQL Error Number: " << error->Number << _T(", Desc: ") << (LPCTSTR)error->Description ) << std::endl;
			}
			
		}

		void Log::FromError(xwostringstream& ss, RawADO::ErrorPtr error)
		{
			LogFromError(ss, error);
		}

		void Log::FromError(xostringstream& ss, RawADO::ErrorPtr error)
		{
			LogFromError(ss, error);
		}
	}
}