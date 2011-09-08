#include "stdafx.h"
#include "IniSection.h"
#include "IniReader.h"

#include "Path.h"
#include "Environment.h"

namespace Earlgrey
{
	namespace Test
	{
 		TEST(IniSectionTest, CurrentSection)
 		{
 			const _txstring fileName = _T("Earlgrey.Test.ini");
 			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);
 
 			IniReader reader(filePath);
 			ASSERT_TRUE(reader.Open());
 
 			const IniReader::SectionNameContainer& names = reader.SectionNames();
 
 			int count = 0;
 			while(reader.MoveNextSection())
 			{
 				IniSection section = reader.CurrentSection();
 				ASSERT_TRUE(section.Name() == names[count]);
 				
 				count++;
 			}
 		}
 
 		TEST(IniSectionTest, ReadString)
 		{
 			const _txstring fileName = _T("Earlgrey.Test.ini");
 			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);
 
 			IniReader reader(filePath);
 			ASSERT_TRUE(reader.Open());
 
 			ASSERT_TRUE(reader.MoveNextSection());
 			IniSection section = reader.CurrentSection();
 
 			_txstring value1 = section.Read<_txstring>(_T("key1"));
 			ASSERT_TRUE(value1 == _T("value1"));

			ASSERT_THROW(section.Read<_txstring>(_T("nokey")), std::exception);
 		}

		TEST(IniSectionTest, ReadLongString)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			_txstring value1 = reader[_T("test5")][_T("connection_string")].GetValue<_txstring>();
			ASSERT_TRUE(value1.length() > 100);
		}

		TEST(IniSectionTest, ReadInt)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			ASSERT_TRUE(reader.MoveNextSection());
			IniSection section = reader.CurrentSection();

			const int valueExpected = 12345;
			const int valueInt = section.Read<int>(_T("keyInt"));//, _T("")); // todo 개선하기
			ASSERT_EQ(valueInt, valueExpected);
		}

		TEST(IniSectionTest, ReadDouble)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			ASSERT_TRUE(reader.MoveNextSection());
			IniSection section = reader.CurrentSection();

			const double valueExpected = 12.3456;
			const double valueDouble = section.Read<double>(_T("keyDouble"));
			ASSERT_DOUBLE_EQ(valueDouble, valueExpected);
		}

		TEST(IniSectionTest, ReadFloat)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			ASSERT_TRUE(reader.MoveNextSection());
			IniSection section = reader.CurrentSection();

			const float valueExpected = 12.34567f;
			const float valueFloat = section.Read<float>(_T("keyFloat"));
			ASSERT_DOUBLE_EQ(valueFloat, valueExpected);
		}

		TEST(IniSectionTest, ReadWithInvalidKey)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			ASSERT_TRUE(reader.MoveNextSection());
			IniSection section = reader.CurrentSection();

			ASSERT_THROW(section.Read<float>(_T("~~~~")), std::exception);
		}

		TEST(IniSectionTest, GetValueUsingSquareBracketOperator)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			ASSERT_TRUE(reader.MoveNextSection());
			IniSection section = reader.CurrentSection();

			const _txstring valueExpected = _T("12.34567");
			const _txstring valueStr = section[_T("keyFloat")].GetValue<_txstring>();
			ASSERT_TRUE(valueExpected == valueStr);
		}

		TEST(IniSectionTest, GetValueUsingSquareBracketOperator2)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			const INT64 valueExpected = 4300000000;
			const INT64 valueInt64 = reader[_T("test4")][_T("keyInt64")].GetValue<INT64>();
			ASSERT_EQ(valueExpected, valueInt64);
		}

		TEST(IniSectionTest, TypeConverter)
		{
			const _txstring src = _T("4300000000");
			IniSection::TypeConverter converter(src);

			const _txstring valueStr = *converter;
			ASSERT_TRUE(valueStr == src);
		}
	}
}