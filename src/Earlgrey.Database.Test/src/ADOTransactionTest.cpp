#include "stdafx.h"
#include "ADOTransaction.h"

#include "ADOEnvironmentTest.h"

#include "ADOConnection.h"
#include "ADOCommand.h"
#include "ADODataReader.h"

#include "DatabaseTestAppSettings.h"


namespace Earlgrey {
namespace ADO {
namespace Test {

	class ADOTransactionTest : public ADOEnvironmentTest
	{
	};

	TEST_F(ADOTransactionTest, Commit)
	{
		const _txstring& connectionString = DatabaseTestAppSettingsSingleton::Instance().ConnectionString();

		Connection connection;
		ASSERT_TRUE2(connection.Open(connectionString));
		ASSERT_TRUE2(connection.IsConnected());

		// ���� �� Ȯ���ϱ�
		const _txstring selectText = _T("select top 1 unique_key from single_column_table");
		Command selectCmd(selectText, connection);

		const long uniqueKey = selectCmd.ExecuteScalar<long>();

		// Ʈ����� ����
		std::tr1::shared_ptr<Transaction> transaction = connection.BeginTransaction();

		// �� ������ ������Ʈ �ϱ�
		const long uniqueKeyUpdated = uniqueKey + 1;

		_txstringstream ss;
		ss << _T("update single_column_table set unique_key=") << uniqueKeyUpdated;

		const _txstring updateText = ss.str();
		Command updateCmd(updateText, connection);

		updateCmd.ExecuteNonQuery();

		// ������Ʈ�� ���� Ȯ���ϱ�
		long uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
		ASSERT_EQ(uniqueKeyUpdated, uniqueKeyRetrieved);

		// Ŀ��
		transaction->Commit();

		// Ŀ���ϰ� �� Ȯ���ϱ�
		uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
		ASSERT_EQ(uniqueKeyUpdated, uniqueKeyRetrieved);

		// �� �� �̻� Ŀ���ϸ� ���� �߻�
		ASSERT_THROW(transaction->Commit(), std::exception);

		// Ŀ���� �Ŀ� �ѹ��ص� ���� �߻�
		ASSERT_THROW(transaction->Rollback(), std::exception);
	}

	TEST_F(ADOTransactionTest, Rollback)
	{
		const _txstring& connectionString = DatabaseTestAppSettingsSingleton::Instance().ConnectionString();

		Connection connection;
		ASSERT_TRUE2(connection.Open(connectionString));
		ASSERT_TRUE2(connection.IsConnected());

		// ���� �� Ȯ���ϱ�
		const _txstring selectText = _T("select top 1 unique_key from single_column_table");
		Command selectCmd(selectText, connection);

		const long uniqueKey = selectCmd.ExecuteScalar<long>();

		// Ʈ����� ����
		std::tr1::shared_ptr<Transaction> transaction = connection.BeginTransaction();

		// �� ������ ������Ʈ �ϱ�
		const long uniqueKeyUpdated = uniqueKey + 1;

		_txstringstream ss;
		ss << _T("update single_column_table set unique_key=") << uniqueKeyUpdated;

		const _txstring updateText = ss.str();
		Command updateCmd(updateText, connection);

		updateCmd.ExecuteNonQuery();

		// ������Ʈ�� ���� Ȯ���ϱ�
		long uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
		ASSERT_EQ(uniqueKeyUpdated, uniqueKeyRetrieved);

		// �ѹ�
		transaction->Rollback();

		// �ѹ��ϰ� �� Ȯ���ϱ�
		uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
		ASSERT_EQ(uniqueKey, uniqueKeyRetrieved);


		// �� �� �̻� �ѹ��ϸ� ���� �߻�
		ASSERT_THROW(transaction->Rollback(), std::exception);

		// Ŀ���� �Ŀ� Ŀ���ص� ���� �߻�
		ASSERT_THROW(transaction->Commit(), std::exception);
	}

	TEST_F(ADOTransactionTest, AutoRollback)
	{
		const _txstring& connectionString = DatabaseTestAppSettingsSingleton::Instance().ConnectionString();

		Connection connection;
		ASSERT_TRUE2(connection.Open(connectionString));
		ASSERT_TRUE2(connection.IsConnected());

		
		// ���� �� Ȯ���ϱ�
		const _txstring selectText = _T("select top 1 unique_key from single_column_table");
		Command selectCmd(selectText, connection);

		const long uniqueKey = selectCmd.ExecuteScalar<long>();

		{
			// Ʈ����� ����
			std::tr1::shared_ptr<Transaction> transaction = connection.BeginTransaction();

			// �� ������ ������Ʈ �ϱ�
			const long uniqueKeyUpdated = uniqueKey + 1;

			_txstringstream ss;
			ss << _T("update single_column_table set unique_key=") << uniqueKeyUpdated;

			const _txstring updateText = ss.str();
			Command updateCmd(updateText, connection);

			updateCmd.ExecuteNonQuery();

			// ������Ʈ�� ���� Ȯ���ϱ�
			long uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
			ASSERT_EQ(uniqueKeyUpdated, uniqueKeyRetrieved);
		}

		// ���뿡�� �ڵ� �ѹ��� �Ǿ�� ....

		// �ѹ��ϰ� �� Ȯ���ϱ�
		long uniqueKeyRetrieved = selectCmd.ExecuteScalar<long>();
		ASSERT_EQ(uniqueKey, uniqueKeyRetrieved);
	}

}
}
}