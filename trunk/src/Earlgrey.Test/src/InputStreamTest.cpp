#include "stdafx.h"
#include "shared_ptr_helper.h"
#include "NetworkBuffer.h"
#include "AsyncStream.h"
#include "Proactor.h"
#include "InputStream.h"

using namespace Earlgrey;

class ConcreteInputStream : public InputStream 
{
public:
	explicit ConcreteInputStream(std::tr1::shared_ptr<AsyncStream> stream) : InputStream(stream)
	{
	}

protected:
	virtual void Read(char value)
	{
		value;
	}

	virtual void Read(short value)
	{
		value;
	}

	virtual void Read(int value)
	{
		value;
	}

	virtual void Read(__int64& value)
	{
		value;
	}

	virtual void Read(unsigned char value)
	{
		value;
	}

	virtual void Read(unsigned short value)
	{
		value;
	}

	virtual void Read(unsigned int value)
	{
		value;
	}

	virtual void Read(unsigned __int64& value)
	{
		value;
	}

	virtual void Read(std::string& value)
	{
		value;
	}

	virtual void Read(std::wstring& value)
	{
		value;
	}

};

TEST(InputStreamTest, OperatorTest)
{
	std::tr1::shared_ptr<AsyncStream> stream = make_ptr(new (alloc<AsyncStream>()) AsyncStream());
	ConcreteInputStream is(stream);

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

	is >> c >> i16 >> i32 >> i64 >> uc >> ui16 >> ui32 >> ui64 >> str >> wstr;
}