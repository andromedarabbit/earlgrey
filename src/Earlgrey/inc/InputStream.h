#pragma once

namespace Earlgrey {

	class AsyncStream;

	class InputStream
	{
	public:
		explicit InputStream(std::tr1::shared_ptr<AsyncStream> stream) : _Stream( stream )
		{
		}

		template<class T>
		friend InputStream& operator>>(InputStream& istream, T& value)
		{
			istream.Read( value );
			return istream;
		}

	protected:
		virtual void Read(char& value) = 0;
		virtual void Read(short& value) = 0;
		virtual void Read(int& value) = 0;
		virtual void Read(__int64& value) = 0;
		virtual void Read(unsigned char& value) = 0;
		virtual void Read(unsigned short& value) = 0;
		virtual void Read(unsigned int& value) = 0;
		virtual void Read(unsigned __int64& value) = 0;
		virtual void Read(std::string& value) = 0;
		virtual void Read(std::wstring& value) = 0;

	protected:
		std::tr1::shared_ptr<AsyncStream> _Stream;
	};

}