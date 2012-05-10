#pragma once
#include "Uncopyable.h"
#include "ADO.h"
#include "txsstream.h"
#include "txstring.h"

namespace Earlgrey
{
	namespace ADO
	{
		// TODO: 코드 정리 좀 하자.
		class Log : private Uncopyable
		{
		private:
			explicit Log();

		public:
			template<typename T>
			static std::basic_string<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type > FromSqlError(RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors = TRUE)
			{
				typedef std::basic_ostringstream<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type > ss_type;

				ss_type ss;
				FromSqlError(ss, command, e, clearErrors);
				return ss.str();
			}
			static void FromSqlError(xwostringstream& ss, RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors = TRUE);
			static void FromSqlError(xostringstream& ss, RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors = TRUE);
		
			static xwstring FromSqlErrorW(RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors = TRUE);
			static xstring FromSqlErrorA(RawADO::_CommandPtr command, const _com_error &e, BOOL clearErrors = TRUE);



			template<typename T>
			static std::basic_string<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type > FromSqlError(RawADO::_ConnectionPtr connection, const _com_error &e, BOOL clearErrors = TRUE)
			{
				typedef std::basic_ostringstream<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type > ss_type;

				ss_type ss;
				FromSqlError(ss, connection, e, clearErrors);
				return ss.str();
			}
			static void FromSqlError(xwostringstream& ss, RawADO::_ConnectionPtr connection, const _com_error &e, BOOL clearErrors = TRUE);
			static void FromSqlError(xostringstream& ss, RawADO::_ConnectionPtr connection, const _com_error &e, BOOL clearErrors = TRUE);

			static xwstring FromSqlErrorW(RawADO::_ConnectionPtr connection, const _com_error &e, BOOL clearErrors = TRUE);
			static xstring FromSqlErrorA(RawADO::_ConnectionPtr connection, const _com_error &e, BOOL clearErrors = TRUE);
		

			template<typename T>
			static std::basic_string<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type > FromProviderError(RawADO::_ConnectionPtr connection, BOOL clearErrors = TRUE)
			{
				typedef std::basic_ostringstream<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type > ss_type;

				ss_type ss;
				FromProviderError(ss, connection, clearErrors);
				return ss.str();
			}
			static void FromProviderError(xwostringstream& ss, RawADO::_ConnectionPtr connection, BOOL clearErrors = TRUE);
			static void FromProviderError(xostringstream& ss, RawADO::_ConnectionPtr connection, BOOL clearErrors = TRUE);
			
			
			template<typename T>
			static std::basic_string<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type > FromComError(const _com_error& e)
			{
				typedef std::basic_ostringstream<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type > ss_type;

				ss_type ss;
				FromComError(ss, e);
				return ss.str();
			}
			static void FromComError(xostringstream& ss, const _com_error& e);
			static void FromComError(xwostringstream& ss, const _com_error& e);

			static xwstring FromComErrorW(const _com_error& e);
			static xstring FromComErrorA(const _com_error& e);


			template<typename T>
			static std::basic_string<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type > FromError(RawADO::ErrorPtr error)
			{
				typedef std::basic_ostringstream<T, std::char_traits<T>, typename StlDefaultAllocator<T>::Type > ss_type;

				ss_type ss;
				FromError(ss, error);
				return ss.str();
			}
			static void FromError(xwostringstream& ss, RawADO::ErrorPtr error);
			static void FromError(xostringstream& ss, RawADO::ErrorPtr error);
		};


		inline void TESTHR(HRESULT x) 
		{
			if (FAILED(x))
				_com_issue_error(x);
		};
	}
}