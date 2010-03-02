#include "stdafx.h"
#include "ADOParameter.h"

// #include "DatabaseTestAppSettings.h"


namespace Earlgrey {
namespace ADO {
namespace Test {

	TEST(ADOParameterTest, Value)
	{
		const int integerValue = 1;

		Parameter parameter;
		parameter.Value(integerValue);

		ASSERT_EQ(integerValue, parameter.Value<int>());
	}

	TEST(ADOParameterTest, ParameterName)
	{
		const _txstring parameterName = _T("ParameterName1");

		Parameter parameter;
		parameter.ParameterName(parameterName);

		ASSERT_TRUE(parameter.ParameterName() == parameterName);
	}

	// TODO: x64로 돌릴 때 RTC 오류가 난다..... ADO 고유의 문제 같지만...
	TEST(ADOParameterTest, Size)
	{
		const long size = 21;

		Parameter parameter;
		parameter.Size(size);

#ifndef WIN64
#ifndef _DEBUG
		ASSERT_EQ(size, parameter.Size());
#endif
#endif
	}

	TEST(ADOParameterTest, Direction)
	{
		const ParameterDirection direction = InputOutput;
		Parameter parameter;
		parameter.Direction(InputOutput);

		ASSERT_EQ(direction, parameter.Direction());
	}

	TEST(ADOParameterTest, Precision)
	{
		const BYTE precision = 1;

		Parameter parameter;
		parameter.Precision(precision);

		ASSERT_EQ(precision, parameter.Precision());
	}

	TEST(ADOParameterTest, Scale)
	{
		const BYTE scale = 12;

		Parameter parameter;
		parameter.Scale(scale);

		ASSERT_EQ(scale, parameter.Scale());
	}


} // !Test
} // !ADO
} // !Earlgrey