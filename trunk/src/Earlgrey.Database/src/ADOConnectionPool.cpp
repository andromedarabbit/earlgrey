#include "stdafx.h"
#include "ADOConnectionPool.h"

#include "EarlgreyAssert.h"
#include "StringHelper.h"


namespace Earlgrey {
	namespace ADO {

		ConnectionPool::ConnectionPool()
		{
		}

		ConnectionPool::~ConnectionPool()
		{
			for(Container::iterator it = m_Pool.begin(); it != m_Pool.end(); it++)
			{				
				ConnectionPtr destructor(it->second);
			}
			m_Pool.clear();
		}

		void ConnectionPool::BeginRegister(const TCHAR * connectionString, Callback callback)
		{
			InvokeMethod(&ConnectionPool::Register_, connectionString, callback);
		}

		void ConnectionPool::Register_(const TCHAR * connectionString, Callback callback) // ��ҹ��� ������.
		{
			BOOL retValue = Register_(connectionString);
			if(callback == NULL)
				return;
			callback(retValue, connectionString);
		}

		// void ConnectionPool::Register_(const _txstring& connectionString) // ��ҹ��� ������.
		BOOL ConnectionPool::Register_(const TCHAR * connectionString) // ��ҹ��� ������.
		{
			EARLGREY_ASSERT(String::IsNullOrEmpty(connectionString) ==  FALSE);

			if( m_Pool.find(connectionString) != m_Pool.end() ) // �̹� ��ϵǾ� �ִ� ���
				return TRUE;

			ConnectionPtr connection( new Connection() );
			if(connection->Open(connectionString) == FALSE)
				return FALSE;

			m_Pool[connectionString] = connection;
			return TRUE;
		}

		void ConnectionPool::BeginDeregister(const TCHAR * connectionString, Callback callback)
		{
			InvokeMethod(&ConnectionPool::Deregister_, connectionString, callback);
		}

		void ConnectionPool::Deregister_(const TCHAR * connectionString, Callback callback) // ��ҹ��� ������.
		{
			BOOL retValue = Deregister_(connectionString);
			if(callback == NULL)
				return;
			callback(retValue, connectionString);
		}

		// void ConnectionPool::Deregister_(const _txstring& connectionString)
		BOOL ConnectionPool::Deregister_(const TCHAR * connectionString)
		{
			EARLGREY_ASSERT(String::IsNullOrEmpty(connectionString) ==  FALSE);

			Container::iterator it = m_Pool.find(connectionString);
			if( it == m_Pool.end() )  // ��� �� �� ������ ���
				return FALSE;

			ConnectionPtr destructor(it->second);
			m_Pool.erase(it);
			return TRUE;
		}

	} // !ADO
} // !Earlgrey