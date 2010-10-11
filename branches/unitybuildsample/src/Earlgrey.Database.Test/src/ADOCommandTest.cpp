#include "stdafx.h"
#include "ADOCommand.h"

#include "ADOEnvironmentTest.h"

#include "ADOConnection.h"
#include "ADODataReader.h"

#include "DatabaseTestAppSettings.h"

#include "DateTime.h"

namespace Earlgrey {
namespace ADO {
namespace Test {

	class ADOCommandTest : public ADOEnvironmentTest
	{
	};

	TEST_F(ADOCommandTest, ExecuteReader)
	{
		const _txstring& connectionString = 
			DatabaseTestAppSettingsSingleton::Instance().ConnectionString();

		Connection connection;
		ASSERT_TRUE(connection.Open(connectionString));
		ASSERT_TRUE(connection.IsConnected());

		const _txstring cmdText = _T("select * from earlgrey_test.simple_table_1");
		Command cmd(cmdText, connection);

		std::tr1::shared_ptr<DataReader> reader = cmd.ExecuteReader();

		int count = 0;
		while(reader->Read())
		{
			long uniqueKey = 0;
			_txstring name;
			DateTime when(DateTime::Now());
			TimeSpan interval;

			*reader >> uniqueKey >> name >> when >> interval;

			// 첫 번째 칼럼 확인
			ASSERT_EQ(count, uniqueKey);

			// 두 번째, 세 번째 칼럼 확인
			if(count == 0)
			{
				ASSERT_TRUE(name == _T("가"));
				ASSERT_EQ(2010, when.Year());
				ASSERT_EQ(43, when.Second());

				ASSERT_EQ(0, interval.Days());
				ASSERT_EQ(0, interval.Hours());
				ASSERT_EQ(0, interval.Minutes());
				ASSERT_EQ(0, interval.Seconds());

			}
			if(count == 1)
			{
				ASSERT_TRUE(name == _T("나"));
				ASSERT_EQ(2011, when.Year());
				ASSERT_EQ(44, when.Second());

				ASSERT_EQ(2, interval.Days());
				ASSERT_EQ(21, interval.Hours());
				ASSERT_EQ(49, interval.Minutes());
				ASSERT_EQ(43, interval.Seconds());
			}
			if(count == 2)
			{
				ASSERT_TRUE(name == _T("다"));
				ASSERT_EQ(2012, when.Year());
				ASSERT_EQ(45, when.Second());

				ASSERT_EQ(5, interval.Days());
				ASSERT_EQ(1, interval.Hours());
				ASSERT_EQ(9, interval.Minutes());
				ASSERT_EQ(3, interval.Seconds());
			}
								
			count++;
		}
		ASSERT_EQ(3, count);
	}

	TEST_F(ADOCommandTest, ExecuteScalar)
	{
		const _txstring& connectionString = DatabaseTestAppSettingsSingleton::Instance().ConnectionString();

		Connection connection;
		ASSERT_TRUE(connection.Open(connectionString));
		ASSERT_TRUE(connection.IsConnected());

		const _txstring cmdText = _T("select `name` from `earlgrey_test`.`simple_table_1` limit 1");
		Command cmd(cmdText, connection);

		_txstring name = cmd.ExecuteScalar<_txstring>();

		ASSERT_TRUE(name == _T("가"));
	}

	TEST_F(ADOCommandTest, ExecuteNonQuery)
	{
		const _txstring& connectionString = DatabaseTestAppSettingsSingleton::Instance().ConnectionString();

		Connection connection;
		ASSERT_TRUE(connection.Open(connectionString));
		ASSERT_TRUE(connection.IsConnected());

		const _txstring selectText = _T("select unique_key from earlgrey_test.single_column_table limit 1");
		Command selectCmd(selectText, connection);

		const long uniqueKey = selectCmd.ExecuteScalar<long>();

		// 새 값으로 업데이트 하기
		const long uniqueKeyExpected = uniqueKey + 1;
			
		_txstringstream ss;
		ss << _T("update `earlgrey_test`.`single_column_table` set unique_key=") << uniqueKeyExpected;

		const _txstring updateText = ss.str();
		Command updateCmd(updateText, connection);

		const LONG recordsAffected = updateCmd.ExecuteNonQuery();
		EXPECT_EQ(0L, recordsAffected);

		// 업데이트된 내용 확인하기
		const long uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
		ASSERT_EQ(uniqueKeyExpected, uniqueKeyRetrieved);

	}

	TEST_F(ADOCommandTest, TryToReadWithWrongColumnName)
	{
		const _txstring& connectionString = DatabaseTestAppSettingsSingleton::Instance().ConnectionString();

		Connection connection;
		ASSERT_TRUE(connection.Open(connectionString));
		ASSERT_TRUE(connection.IsConnected());

		const _txstring cmdText = _T("select invalid_column from earlgrey_test.simple_table_1");
		Command cmd(cmdText, connection);

		ASSERT_THROW(cmd.ExecuteReader(), std::exception);
	}

	TEST_F(ADOCommandTest, TryToReadWithWrongColumnIndex)
	{
		const _txstring& connectionString = DatabaseTestAppSettingsSingleton::Instance().ConnectionString();

		Connection connection;
		ASSERT_TRUE(connection.Open(connectionString));
		ASSERT_TRUE(connection.IsConnected());

		const _txstring cmdText = _T("select * from earlgrey_test.simple_table_1");
		Command cmd(cmdText, connection);

		std::tr1::shared_ptr<DataReader> reader = cmd.ExecuteReader();

		ASSERT_TRUE(reader->Read());

		long uniqueKey = 0;
		_txstring name;
		DateTime when(DateTime::Now());
		TimeSpan interval;
		TimeSpan invalidColumn1;
		TimeSpan invalidColumn2;

		*reader >> uniqueKey >> name >> when >> interval >> invalidColumn1 >> invalidColumn2;
		ASSERT_TRUE(reader->fail());

		ASSERT_THROW(reader->GetValue<int>(32), std::exception);
	}

}
}
}