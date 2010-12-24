#pragma once

namespace Earlgrey {

	class AsyncStream;

	class OutputStream
	{
	public:
		explicit OutputStream(std::tr1::shared_ptr<AsyncStream> stream);

		template<class T>
		friend OutputStream& operator<<(OutputStream& ostream, T& value)
		{
			ostream.Write( value );
			return ostream;
		}

		void Flush();

	protected:
		virtual void Write(char& value) = 0;
		virtual void Write(short& value) = 0;
		virtual void Write(int& value) = 0;
		virtual void Write(__int64& value) = 0;
		virtual void Write(unsigned char& value) = 0;
		virtual void Write(unsigned short& value) = 0;
		virtual void Write(unsigned int& value) = 0;
		virtual void Write(unsigned __int64& value) = 0;
		virtual void Write(std::string& value) = 0;
		virtual void Write(std::wstring& value) = 0;

	protected:
		std::tr1::shared_ptr<AsyncStream> _Stream;
	};

}