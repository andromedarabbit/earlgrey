#pragma once
#include "ADO.h"
#include "StringHelper.h"
#include "numeric_cast.hpp"

#include "DateTime.h"
#include "DateTimeKind.h"
#include "TimeSpan.h"

#include "CommandType.h"

namespace Earlgrey {
	namespace ADO {

		template<typename Target, typename Source>
		inline Target database_cast(const Source& arg)
		{
			return Target(arg);
		}

		template<>
		inline long database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_I4);
			
			return arg.lVal;
		}

		template<>
		inline float database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_R4);
			return arg.fltVal;
		}

		template<>
		inline double database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_R8);
			return arg.dblVal;
		}

		template<>
		inline SHORT database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_I2);
			return arg.iVal;
		}


		template<>
		inline CY database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_CY);
			return arg.cyVal;
		}

		template<>
		inline _bstr_t database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_BSTR);
			return arg.bstrVal;
		}

		template<>
		inline const WCHAR * database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_BSTR);
			
			if(arg.vt == VT_NULL)
				return NULL;
			
			return static_cast<const WCHAR *>(_bstr_t(arg.bstrVal));
		}

		// TODO: 다른 string도 함께 쓸 수 있게... 
		template<>
		inline xwstring database_cast(const _variant_t& arg)
		{
			const WCHAR * charArray = database_cast<const WCHAR*>(arg);
			return xwstring(charArray);
		}

		template<>
		inline const CHAR * database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_BSTR);

			// TODO: 문서에 기술된 바와 다르 게 아래 코드가 작동 안 한다.
			/// const CHAR * ansi = static_cast<const CHAR *>(_bstr_t(arg.bstrVal));
			const WCHAR* unicode = database_cast<const WCHAR *>(arg);
			const int unicodeBytes = EARLGREY_NUMERIC_CAST<int>(
				wcslen(unicode) * sizeof(WCHAR)
				);
			return String::FromUnicode(unicode, unicodeBytes);
		}

		// TODO: 다른 string도 함께 쓸 수 있게... 
		template<>
		inline xstring database_cast(const _variant_t& arg)
		{
			const CHAR * charArray = database_cast<const CHAR*>(arg);
			return xstring(charArray);
		}
		
		template<>
		inline bool database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_BOOL);

#pragma warning(push)
#pragma warning(disable: 4800)
			return static_cast<bool>(arg.boolVal);
#pragma warning(pop)
		}


		template<>
		inline DECIMAL database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_DECIMAL);
			return arg.decVal;
		}

		template<>
		inline BYTE database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_UI1);
			return arg.bVal;
		}

		template<>
		inline char database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_I1);
			return arg.cVal;
		}

		template<>
		inline USHORT database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_UI2);
			return arg.uiVal;
		}

		template<>
		inline ULONG database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_UI4);
			return arg.ulVal;
		}

		template<>
		inline INT database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_INT);
			return arg.intVal;
		}

		template<>
		inline UINT database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_UINT);
			return arg.uintVal;
		}

		template<>
		inline LONGLONG database_cast<LONGLONG, _variant_t>(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_I8);
			return arg.llVal;
		}

		template<>
		inline ULONGLONG database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_UI8);
			return arg.ullVal;
		}

		
		template<>
		inline DateTime database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_DATE);
			
			SYSTEMTIME systemTime;
			if(::VariantTimeToSystemTime(arg.date, &systemTime) == 0)
				throw std::exception("");

			return DateTime(
				systemTime.wYear
				, systemTime.wMonth
				, systemTime.wDay
				, systemTime.wHour
				, systemTime.wMinute
				, systemTime.wSecond
				, systemTime.wMilliseconds
				, DATETIMEKIND_LOCAL
				);
		}

		template<>
		inline _variant_t database_cast<_variant_t, DateTime>(const DateTime& arg)
		{
			SYSTEMTIME systemTime = {
				EARLGREY_NUMERIC_CAST<WORD>(arg.Year())
				, EARLGREY_NUMERIC_CAST<WORD>(arg.Month())
				, 0
				, EARLGREY_NUMERIC_CAST<WORD>(arg.Day())
				, EARLGREY_NUMERIC_CAST<WORD>(arg.Hour())
				, EARLGREY_NUMERIC_CAST<WORD>(arg.Minute())
				, EARLGREY_NUMERIC_CAST<WORD>(arg.Second())
				, EARLGREY_NUMERIC_CAST<WORD>(arg.Millisecond())
				};

			double vTime;
			if(::SystemTimeToVariantTime(&systemTime, &vTime) == 0)
				throw std::exception("");

			return _variant_t(vTime, VT_DATE);
		}

		template<>
		inline TimeSpan database_cast(const _variant_t& arg)
		{
			EARLGREY_ASSERT(arg.vt != VT_EMPTY);
			EARLGREY_ASSERT(arg.vt == VT_DATE);

			static const TimeSpan::TickType ZERO_TICK_FOR_DB = 94353120000000000; //1072620576000000000;

			SYSTEMTIME systemTime;
			if(::VariantTimeToSystemTime(arg.date, &systemTime) == 0)
				throw std::exception("");

			FILETIME fileTime;
			if(::SystemTimeToFileTime(&systemTime, &fileTime) == 0)
				throw std::exception("");

			const TimeSpan::TickType newTimeSpan = *reinterpret_cast<TimeSpan::TickType*>(&fileTime) - ZERO_TICK_FOR_DB;
			return TimeSpan(newTimeSpan);
		}
		
		template<>
		inline _variant_t database_cast<_variant_t, TimeSpan>(const TimeSpan& arg)
		{
			static const TimeSpan::TickType ZERO_TICK_FOR_DB = 94353120000000000; //1072620576000000000;

			TimeSpan::TickType newTimeSpan = arg.Ticks() + ZERO_TICK_FOR_DB;
			FILETIME fileTime = *reinterpret_cast<LPFILETIME>(&newTimeSpan);

			SYSTEMTIME systemTime;
			if(::FileTimeToSystemTime(&fileTime, &systemTime) == 0)
				throw std::exception("");

			double vTime;
			if(::SystemTimeToVariantTime(&systemTime, &vTime) == 0)
				throw std::exception("");

			return _variant_t(vTime, VT_DATE);
		}

		// ADO Native types
		// TODO:: 임시로 함수 이름을 지음
		template<>
		inline CommandType database_cast<CommandType, RawADO::CommandTypeEnum>(
			const RawADO::CommandTypeEnum& arg
			)
		{
			if(arg == RawADO::adCmdText)
				return Text;

			if(arg == RawADO::adCmdStoredProc)
				return StoredProcedure;

			throw std::exception("");
		}

		template<>
		inline RawADO::CommandTypeEnum database_cast<RawADO::CommandTypeEnum, CommandType>(
			const CommandType& arg
			)
		{
			if(arg == Text)
				return RawADO::adCmdText;

			if(arg == StoredProcedure)
				return RawADO::adCmdStoredProc;

			throw std::exception("");
		}


	} // !ADO
} // !Earlgrey