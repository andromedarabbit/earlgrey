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
 					
 					// ù ��° Į�� Ȯ��
 					ASSERT_EQ(count, uniqueKey);
 
 					// �� ����, �� ��° Į�� Ȯ��
 					if(count == 0)
 					{
 						ASSERT_TRUE(name == _T("��"));
 						ASSERT_EQ(2010, when.Year());
 						ASSERT_EQ(43, when.Second());
 					}
 					if(count == 1)
 					{
 						ASSERT_TRUE(name == _T("��"));
 						ASSERT_EQ(2011, when.Year());
 						ASSERT_EQ(44, when.Second());
 					}
 					if(count == 2)
 					{
 						ASSERT_TRUE(name == _T("��"));
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
 
 				ASSERT_TRUE(name == _T("��"));
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

				// �� ������ ������Ʈ �ϱ�
				const long uniqueKeyExpected = uniqueKey + 1;
					
				_txstringstream ss;
				ss << _T("update `earlgrey_test`.`single_column_table` set unique_key=") << uniqueKeyExpected;

				const _txstring updateText = ss.str();
				Command updateCmd(updateText, connection);

				updateCmd.ExecuteNonQuery();

				// ������Ʈ�� ���� Ȯ���ϱ�
				const long uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
				ASSERT_EQ(uniqueKeyExpected, uniqueKeyRetrieved);

			}
		
		}
	}
}