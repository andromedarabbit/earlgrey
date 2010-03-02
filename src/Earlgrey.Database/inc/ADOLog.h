#pragma once
#include "Uncopyable.h"
#include "ADO.h"

namespace Earlgrey
{
	namespace ADO
	{
		class Log : private Uncopyable
		{
		private:
			explicit Log();

		public:
			static void ReportSqlError(RawADO::_CommandPtr command, _com_error &e, BOOL clearErrors = TRUE);
			static void ReportSqlError(RawADO::_ConnectionPtr pConnection, _com_error &e, BOOL clearErrors = TRUE);
			static void ReportProviderError(RawADO::_ConnectionPtr pConnection, BOOL clearErrors = TRUE);
			static void ReportComError(_com_error &e);
			// static void ReportError(struct Error* error);
			static void ReportError(RawADO::ErrorPtr error);
		};

		inline void TESTHR(HRESULT x) 
		{
			if FAILED(x) 
				_com_issue_error(x);
		};
	}
}