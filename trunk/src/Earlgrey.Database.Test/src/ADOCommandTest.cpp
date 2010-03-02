#include "stdafx.h"
#include "ADOCommand.h"

#include "ADOConnection.h"
#include "ADODataReader.h"

#include "DatabaseTestAppSettings.h"

#include "DateTime.h"

namespace Earlgrey {
	namespace ADO {
		namespace Test {

 			TEST(ADOCommandTest, ExecuteReader)
 			{
 				const _txstring& connectionString = DatabaseTestAppSettingsSingleton::Instance().ConnectionString();
 
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
 
 					*reader >> uniqueKey >> name >> when;
 					
 					// 첫 번째 칼럼 확인
 					ASSERT_EQ(count, uniqueKey);
 
 					// 두 번쨰, 세 번째 칼럼 확인
 					if(count == 0)
 					{
 						ASSERT_TRUE(name == _T("가"));
 						ASSERT_EQ(2010, when.Year());
 						ASSERT_EQ(43, when.Second());
 					}
 					if(count == 1)
 					{
 						ASSERT_TRUE(name == _T("나"));
 						ASSERT_EQ(2011, when.Year());
 						ASSERT_EQ(44, when.Second());
 					}
 					if(count == 2)
 					{
 						ASSERT_TRUE(name == _T("다"));
 						ASSERT_EQ(2012, when.Year());
 						ASSERT_EQ(45, when.Second());
 					}
 										
 					count++;
 				}
 				ASSERT_EQ(3, count);
 			}

 			TEST(ADOCommandTest, ExecuteScalar)
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

			TEST(ADOCommandTest, ExecuteNonQuery)
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

				updateCmd.ExecuteNonQuery();

				// 업데이트된 내용 확인하기
				const long uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
				ASSERT_EQ(uniqueKeyExpected, uniqueKeyRetrieved);

			}
		
		}
	}
}