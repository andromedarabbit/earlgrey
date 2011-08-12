#pragma once
#include "Uncopyable.h"
#include "ADO.h"
#include "txsstream.h"
#include "txstring.h"

namespace Earlgrey
{
	namespace ADO
	{
		class Log : private Uncopyable
		{
		private:
			explicit Log();

		public:
			static _txstring FromSqlError(RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors = TRUE);
			static void FromSqlError(_txostringstream& ss, RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors = TRUE);

			static _txstring FromSqlError(RawADO::_ConnectionPtr pConnection, const _com_error &e, BOOL clearErrors = TRUE);
			static void FromSqlError(_txostringstream& ss, RawADO::_ConnectionPtr pConnection, const _com_error &e, BOOL clearErrors = TRUE);

			static _txstring FromProviderError(RawADO::_ConnectionPtr pConnection, BOOL clearErrors = TRUE);
			static void FromProviderError(_txostringstream& ss, RawADO::_ConnectionPtr pConnection, BOOL clearErrors = TRUE);
			
			static _txstring FromComError(const _com_error &e);
			static void FromComError(_txostringstream& ss, const _com_error &e);


			static _txstring FromError(RawADO::ErrorPtr error);
			static void FromError(_txostringstream& ss, RawADO::ErrorPtr error);

		};


		inline void TESTHR(HRESULT x) 
		{
			if FAILED(x) 
				_com_issue_error(x);
		};
	}
}