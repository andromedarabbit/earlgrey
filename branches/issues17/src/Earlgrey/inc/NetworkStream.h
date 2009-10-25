#pragma once

#include "Uncopyable.h"
#include "numeric_cast.hpp"
#include "NetworkBuffer.h"
#include "txstring.h"

namespace Earlgrey {

	struct EndOfStream {};

	template<typename T>
	struct NullWriter
	{
		static BOOL Write(NetworkBuffer&, const T&)
		{
			return TRUE;
		}
	};

	template<typename T>
	struct DefaultWriter
	{
		static BOOL Write(NetworkBuffer& Buffer, const T& Value)
		{
			Buffer.SetValue( reinterpret_cast<const BYTE*>( &Value ), sizeof(T) );
			return TRUE;
		}
	};

	template<>
	struct DefaultWriter<_txstring>
	{
		static BOOL Write(NetworkBuffer& Buffer, const _txstring& Value)
		{
			DWORD Length = Math::numeric_cast<DWORD>( Value.size() );
			Buffer.SetValue( reinterpret_cast<const BYTE*>( &Length ), sizeof(DWORD) );
			if (Length > 0)
			{
				Buffer.SetValue( reinterpret_cast<const BYTE*>( Value.c_str() ), Length * sizeof(TCHAR) );
			}
			return TRUE;
		}
	};

	template<>
	struct DefaultWriter<NetworkBuffer>
	{
		static BOOL Write(NetworkBuffer& Buffer, const NetworkBuffer& Value)
		{
			Buffer = Value;
			return TRUE;
		}
	};

	struct StringWriterHelper
	{
		template<typename T>
		static _txstring IntToString(T Value)
		{
			TCHAR tmp[100] = {0};
			EARLGREY_STATIC_ASSERT( std::numeric_limits<T>::is_integer );
			_itot_s( Value, tmp, 10 );
			return _txstring( tmp );
		}

		static _txstring Int64ToString(__int64 Value)
		{
			TCHAR tmp[100] = {0};
			_i64tot_s( Value, tmp, _countof(tmp), 10 );
			return _txstring( tmp );
		}

		static _txstring UInt64ToString(unsigned __int64 Value)
		{
			TCHAR tmp[100] = {0};
			_ui64tot_s( Value, tmp, _countof(tmp), 10 );
			return _txstring( tmp );
		}

		//! \todo modify to set ANSI characters into NetworkBuffer
		static void WriteString(NetworkBuffer& Buffer, const _txstring& Str)
		{
			Buffer.SetValue( reinterpret_cast<const BYTE*>( Str.c_str() ), Math::numeric_cast<DWORD>(Str.length() * sizeof(TCHAR)) );
		}

		template<typename T>
		static T StringToInt(const _txstring& Value)
		{
			T Result = _ttoi( Value.c_str() );
			return Result;
		}

		static __int64 StringToIn64(const _txstring& Value)
		{
			__int64 Result = _ttoi64( Value.c_str() );
			return Result;
		}

		static unsigned __int64 StringToUInt64(const _txstring& Value)
		{
			return StringToIn64( Value );
		}

		static BOOL ReadString(NetworkBuffer& Buffer, _txstring& Str)
		{
			Buffer;
			Str;
			return TRUE;
		}
	};

	template<typename T>
	struct StringWriter
	{
		static BOOL Write(NetworkBuffer& Buffer, const T& Value)
		{
			_txstring Str = StringWriterHelper::IntToString( Value );
			StringWriterHelper::WriteString( Buffer, Str );
			return TRUE;
		}
	};

	template<>
	struct StringWriter<_txstring>
	{
		static BOOL Write(NetworkBuffer& Buffer, const _txstring& Str)
		{
			StringWriterHelper::WriteString( Buffer, _txstring(_T("\"")) );
			StringWriterHelper::WriteString( Buffer, Str );
			StringWriterHelper::WriteString( Buffer, _txstring(_T("\"")) );
			return TRUE;
		}
	};

	template<>
	struct StringWriter<NetworkBuffer>
	{
		static BOOL Write(NetworkBuffer& Buffer, const NetworkBuffer& Value)
		{
			Buffer = Value;
			return TRUE;
		}
	};

	template<template<class> class Writer = DefaultWriter> 
	class NetworkOutputStream : private Uncopyable
	{
	public:
		template<typename T>
		FORCEINLINE friend NetworkOutputStream& operator<<(NetworkOutputStream& OutputStream, const T& Value)
		{
			Writer<T>::Write( OutputStream._Buffer, Value );
			return OutputStream;
		}

		FORCEINLINE friend NetworkOutputStream& operator<<(NetworkOutputStream& OutputStream, const EndOfStream&)
		{
			OutputStream.Flush();
			return OutputStream;
		}

		void Flush()
		{
		}

	private:
		NetworkBuffer _Buffer;
	};

	template<typename T>
	struct DefaultReader
	{
		static BOOL Read(NetworkBuffer& Buffer, DWORD& Offset, T& Value)
		{
			return Buffer.GetValue( Offset, reinterpret_cast<BYTE*>(&Value), sizeof(T) );
		}
	};

	template<>
	struct DefaultReader<_txstring>
	{
		static BOOL Read(NetworkBuffer& Buffer, DWORD& Offset, _txstring& Value)
		{
			DWORD Length = 0;
			BOOL Result = Buffer.GetValue( Offset, reinterpret_cast<BYTE*>(&Length), sizeof(DWORD) );
			if (!Result)
			{
				return FALSE;
			}

			if (0 == Length)
			{
				return TRUE;
			}

			std::vector<TCHAR> ReadStr;	//! \todo xvector·Î º¯°æ
			ReadStr.reserve( Length );
			Result = Buffer.GetValue( Offset, reinterpret_cast<BYTE*>(&ReadStr[0]), Length * sizeof(TCHAR) );
			if (!Result)
			{
				return FALSE;
			}

			Value.assign( &ReadStr[0], Length );
			return TRUE;
		}
	};

	template<typename T>
	struct StringReader
	{
		static BOOL Read(NetworkBuffer& Buffer, DWORD& Offset, T& Value)
		{
			return TRUE;
		}
	};

	template<template<class> class Reader = DefaultReader>
	class NetworkInputStream : private Uncopyable
	{
	public:
		NetworkInputStream() : _IsValidStream(TRUE), _Offset(0)
		{
		}

		BOOL IsValid() const { return _IsValidStream; }

		template<typename T>
		FORCEINLINE friend NetworkInputStream& operator>>(NetworkInputStream& InputStream, T& Value)
		{
			InputStream._IsValidStream &= Reader<T>::Read( InputStream._Buffer, InputStream._Offset, Value );
			return InputStream;
		}

	private:
		NetworkBuffer _Buffer;
		DWORD _Offset;
		BOOL _IsValidStream;
	};
}