#include "stdafx.h"
#include "ADOConnection.h"

#include "ADOEnvironmentTest.h"
#include "DatabaseTestAppSettings.h"

// юс╫ц
#include "ADOLog.h"


namespace Earlgrey {
namespace ADO {
namespace Test {

	class ADOConnectionTest : public ADOEnvironmentTest
	{
	};

	TEST_F(ADOConnectionTest, NotConnected)
	{
		Connection connection;
		ASSERT_FALSE(connection.IsConnected());
	}

	TEST_F(ADOConnectionTest, ConnectionFail)
	{
		const _txstring invalidConnectionString;

		Connection connection;
		ASSERT_FALSE(connection.Open(invalidConnectionString));
		ASSERT_FALSE(connection.IsConnected());
	}

	TEST_F(ADOConnectionTest, OpenLocalMySQLUsingODBC)
	{
		const _txstring& connectionString = DatabaseTestAppSettingsSingleton::Instance().ConnectionString();
		
		Connection connection;
		ASSERT_TRUE2(connection.Open(connectionString));
		ASSERT_TRUE2(connection.IsConnected());
		connection.Close();
		ASSERT_FALSE(connection.IsConnected());
	}

	TEST_F(ADOConnectionTest, ConnectionTimeout)
	{
		const _txstring& connectionString = DatabaseTestAppSettingsSingleton::Instance().ConnectionString();
		TimeSpan timeout(0, 0, 30);

		Connection connection;
		ASSERT_TRUE2(connection.Open(connectionString, timeout));
		ASSERT_EQ(timeout, connection.ConnectionTimeout());
		connection.Close();
		ASSERT_FALSE(connection.IsConnected());
	}

}
}
}