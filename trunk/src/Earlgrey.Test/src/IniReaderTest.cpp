#include "stdafx.h"
#include "IniReader.h"

#include "IniSection.h"

#include "Path.h"
#include "Environment.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(IniReaderTest, Open)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			ASSERT_THROW(reader.Open(), std::exception);
		}

		TEST(IniReaderTest, ReadSectionNames)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			const IniReader::SectionNameContainer& names = reader.SectionNames();
			ASSERT_GT(names.size(), static_cast<size_t>(0));
		}

		TEST(IniReaderTest, MoveNextSection)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			size_t count = 0;
			while(reader.MoveNextSection())
			{
				count++;
			}

			const IniReader::SectionNameContainer& names = reader.SectionNames();
			ASSERT_EQ(names.size(), count);	
		}

		TEST(IniReaderTest, ReadEmptyIniFile)
		{
			const _txstring fileName = _T("Earlgrey.Test.Empty.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			EXPECT_FALSE(reader.Open());

			ASSERT_FALSE(reader.MoveNextSection());
		}

		TEST(IniReaderTest, MoveSection)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			ASSERT_TRUE(reader.MoveSection(_T("test4")));
			
			IniSection section = reader.CurrentSection();

			const INT64 valueExpected = 4300000000;
			const INT64 valueInt64 = section.Read<INT64>(_T("keyInt64"));
			ASSERT_EQ(valueInt64, valueExpected);
		}

		TEST(IniReaderTest, MoveToLastSection)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			ASSERT_TRUE(reader.MoveSection(_T("test5")));

			IniSection section = reader.CurrentSection();

			const INT valueExpected = -1;
			const INT valueInt = section.Read<INT>(_T("keyLessThanZero"));
			ASSERT_EQ(valueInt, valueExpected);
		}

		TEST(IniReaderTest, GetSectionUsingSquareBracketOperator)
		{
			const _txstring fileName = _T("Earlgrey.Test.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			IniSection section = reader[_T("test5")];

			const INT64 valueExpected = -1;
			const INT64 valueInt64 = section.Read<INT>(_T("keyLessThanZero"));
			ASSERT_EQ(valueInt64, valueExpected);
		}

		// TODO: support unicode files.
		TEST(IniReaderTest, NotAbleToReadUnicodeFile)
		{
			const _txstring fileName = _T("Earlgrey.Test.In.Unicode.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_FALSE(reader.Open());

// 			IniSection section1 = reader[_T("한글 섹션 1")];
// 			const _tstring value1 = section1.Read<_tstring>(_T("한글 키 1"));
// 			ASSERT_TRUE(_T("한글 값") == value1);
		}

		TEST(IniReaderTest, HandleCommentedItems)
		{
			const _txstring fileName = _T("Earlgrey.Test.Comments.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			IniSection section1 = reader[_T("Section1")];

			// ;CommentedKey1=CommentedValue1
			ASSERT_THROW(section1.Read<_tstring>(_T("CommentedKey1")), std::exception);
			ASSERT_THROW(section1.Read<_tstring>(_T(";CommentedKey1")), std::exception);

			const _tstring value2 = section1.Read<_tstring>(_T("Key2"));
			ASSERT_TRUE(_T(";InlineCommentDelimiterIsNotTreatedAsComment") == value2);

			const _tstring value3 = section1.Read<_tstring>(_T("Key3"));
			ASSERT_TRUE(_T("Value3") == value3);


			// Section 2
			IniSection section2 = reader[_T("Section2")];

			// "//" is not treated as a comment delimiter
			ASSERT_THROW(section1.Read<_tstring>(_T("Key4")), std::exception);

			const _tstring value4 = section2.Read<_tstring>(_T("//Key4"));
			ASSERT_TRUE(_T("Value4") == value4);

			// "#" is not treated as a comment delimiter
			ASSERT_THROW(section1.Read<_tstring>(_T("Key5")), std::exception);

			const _tstring value5 = section2.Read<_tstring>(_T("#Key5"));
			ASSERT_TRUE(_T("Value5") == value5);
		}
		
		TEST(IniReaderTest, ReadSectionKeyValueWithBlanks)
		{
			// [Section 3]
			// Key with blanks = value	with	blanks
			const _txstring fileName = _T("Earlgrey.Test.Comments.ini");
			const _txstring filePath = Path::Combine(Environment::BaseDirectory(), fileName);

			IniReader reader(filePath);
			ASSERT_TRUE(reader.Open());

			// able to read a section with a blank
			const TCHAR * section3Name = _T("Section 3");
			IniSection section3 = reader[section3Name];
			ASSERT_TRUE(section3Name == section3.Name());

			// able to read a key and value with a blank
			_txstring valueWithBlanks = section3.Read<_txstring>(_T("Key with blanks"));
			ASSERT_TRUE(_T("value	with	blanks") == valueWithBlanks);

			_txstring valueWithBlanksInKorean = section3.Read<_txstring>(_T("한글 키"));
			ASSERT_TRUE(_T("한글 값") == valueWithBlanksInKorean);
		}


	}
}