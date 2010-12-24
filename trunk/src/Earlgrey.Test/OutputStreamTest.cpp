#include "stdafx.h"
#include "shared_ptr_helper.h"
#include "OutputStream.h"
#include "NetworkBuffer.h"
#include "Proactor.h"
#include "AsyncStream.h"

using namespace Earlgrey;

class ConcreteOutputStream : public OutputStream 
{
public:
	explicit ConcreteOutputStream(std::tr1::shared_ptr<AsyncStream> stream) : OutputStream(stream)
	{
	}

protected:
	virtual void Write(char& value)
	{
		value;
	}

	virtual void Write(short& value)
	{
		value;
	}

	virtual void Write(int& value)
	{
		value;
	}

	virtual void Write(__int64& value)
	{
		value;
	}

	virtual void Write(unsigned char& value)
	{
		value;
	}

	virtual void Write(unsigned short& value)
	{
		value;
	}

	virtual void Write(unsigned int& value)
	{
		value;
	}

	virtual void Write(unsigned __int64& value)
	{
		value;
	}

	virtual void Write(std::string& value)
	{
		value;
	}

	virtual void Write(std::wstring& value)
	{
		value;
	}

};

TEST(OutputStreamTest, OperatorTest)
{
	std::tr1::shared_ptr<AsyncStream> stream( new AsyncStream() ) ;
	ConcreteOutputStream os( stream );

	char c = 0;
	short i16 = 0;
	int i32 = 0;
	__int64 i64 = 0;

	unsigned char uc = 0;
	unsigned short ui16 = 0;
	unsigned int ui32 = 0;
	unsigned __int64 ui64 = 0;

	std::string str;
	std::wstring wstr;

	os << c << i16 << i32 << i64 << uc << ui16 << ui32 << ui64 << str << wstr;
}