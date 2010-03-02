#include "stdafx.h"
#include "ADOTransaction.h"


#include "ADOConnection.h"
#include "ADOCommand.h"
#include "ADODataReader.h"

#include "DatabaseTestAppSettings.h"


namespace Earlgrey {
	namespace ADO {
		namespace Test {


			TEST(ADOTransactionTest, Commit)
			{
				const _txstring& connectionString = DatabaseTestAppSettingsSingleton::Instance().ConnectionString();

				Connection connection;
				ASSERT_TRUE(connection.Open(connectionString));
				ASSERT_TRUE(connection.IsConnected());

				// 기존 값 확인하기
				const _txstring selectText = _T("select unique_key from earlgrey_test.single_column_table limit 1");
				Command selectCmd(selectText, connection);

				const long uniqueKey = selectCmd.ExecuteScalar<long>();

				// 트랜잭션 시작
				std::tr1::shared_ptr<Transaction> transaction = connection.BeginTransaction();

				// 새 값으로 업데이트 하기
				const long uniqueKeyUpdated = uniqueKey + 1;

				_txstringstream ss;
				ss << _T("update `earlgrey_test`.`single_column_table` set unique_key=") << uniqueKeyUpdated;

				const _txstring updateText = ss.str();
				Command updateCmd(updateText, connection);

				updateCmd.ExecuteNonQuery();

				// 업데이트된 내용 확인하기
				long uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
				ASSERT_EQ(uniqueKeyUpdated, uniqueKeyRetrieved);

				// 커밋
				transaction->Commit();

				// 커밋하고 값 확인하기
				uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
				ASSERT_EQ(uniqueKeyUpdated, uniqueKeyRetrieved);

				// 두 번 이상 커밋하면 예외 발생
				ASSERT_THROW(transaction->Commit(), std::exception);

				// 커밋한 후에 롤백해도 예외 발생
				ASSERT_THROW(transaction->Rollback(), std::exception);
			}

			TEST(ADOTransactionTest, Rollback)
			{
				const _txstring& connectionString = DatabaseTestAppSettingsSingleton::Instance().ConnectionString();

				Connection connection;
				ASSERT_TRUE(connection.Open(connectionString));
				ASSERT_TRUE(connection.IsConnected());

				// 기존 값 확인하기
				const _txstring selectText = _T("select unique_key from earlgrey_test.single_column_table limit 1");
				Command selectCmd(selectText, connection);

				const long uniqueKey = selectCmd.ExecuteScalar<long>();

				// 트랜잭션 시작
				std::tr1::shared_ptr<Transaction> transaction = connection.BeginTransaction();

				// 새 값으로 업데이트 하기
				const long uniqueKeyUpdated = uniqueKey + 1;

				_txstringstream ss;
				ss << _T("update `earlgrey_test`.`single_column_table` set unique_key=") << uniqueKeyUpdated;

				const _txstring updateText = ss.str();
				Command updateCmd(updateText, connection);

				updateCmd.ExecuteNonQuery();

				// 업데이트된 내용 확인하기
				long uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
				ASSERT_EQ(uniqueKeyUpdated, uniqueKeyRetrieved);

				// 롤백
				transaction->Rollback();

				// 롤백하고 값 확인하기
				uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
				ASSERT_EQ(uniqueKey, uniqueKeyRetrieved);


				// 두 번 이상 롤백하면 예외 발생
				ASSERT_THROW(transaction->Rollback(), std::exception);

				// 커밋한 후에 커밋해도 예외 발생
				ASSERT_THROW(transaction->Commit(), std::exception);
			}

			TEST(ADOTransactionTest, AutoRollback)
			{
				const _txstring& connectionString = DatabaseTestAppSettingsSingleton::Instance().ConnectionString();

				Connection connection;
				ASSERT_TRUE(connection.Open(connectionString));
				ASSERT_TRUE(connection.IsConnected());

				
				// 기존 값 확인하기
				const _txstring selectText = _T("select unique_key from earlgrey_test.single_column_table limit 1");
				Command selectCmd(selectText, connection);

				const long uniqueKey = selectCmd.ExecuteScalar<long>();

				{
					// 트랜잭션 시작
					std::tr1::shared_ptr<Transaction> transaction = connection.BeginTransaction();

					// 새 값으로 업데이트 하기
					const long uniqueKeyUpdated = uniqueKey + 1;

					_txstringstream ss;
					ss << _T("update `earlgrey_test`.`single_column_table` set unique_key=") << uniqueKeyUpdated;

					const _txstring updateText = ss.str();
					Command updateCmd(updateText, connection);

					updateCmd.ExecuteNonQuery();

					// 업데이트된 내용 확인하기
					long uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
					ASSERT_EQ(uniqueKeyUpdated, uniqueKeyRetrieved);
				}

				// 이쯤에서 자동 롤백이 되어야 ....

				// 롤백하고 값 확인하기
				long uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
				ASSERT_EQ(uniqueKey, uniqueKeyRetrieved);
			}

		}
	}
}