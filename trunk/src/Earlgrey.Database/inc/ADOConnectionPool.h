#pragma once
#include "xmap.h"
#include "txstring.h"
#include "ADOConnection.h"

#include "taskqueue.h"

#include <Loki/Singleton.h>
#include "NoLock.h"

#include <functional>

namespace Earlgrey {
	namespace ADO {

		//! \ref http://kaistizen.net/EE/index.php/weblog/comments/myth_of_ado_connection_pool/
		class ConnectionPool 
			: public Algorithm::Lockfree::TaskQueue<>
		{
			friend struct Loki::CreateUsingNew<ConnectionPool>;

			typedef std::tr1::shared_ptr<Connection> ConnectionPtr;
			typedef xmap<_txstring, ConnectionPtr>::Type Container;

		public:
			typedef std::tr1::function< void(BOOL, const TCHAR *) > Callback;

			explicit ConnectionPool();
			~ConnectionPool();		

			void BeginRegister(const TCHAR * connectionString, Callback callback); // 대소문자 구분함.
			void BeginDeregister(const TCHAR * connectionString, Callback callback);
		
		private: // methods
			BOOL Register_(const TCHAR * connectionString); 
			void Register_(const TCHAR * connectionString, Callback callback); 
			BOOL Deregister_(const TCHAR * connectionString);
			void Deregister_(const TCHAR * connectionString, Callback callback);

		private:
			Container m_Pool;
		};

		typedef 
			// Loki::SingletonHolder<ConnectionPool, Loki::CreateUsingNew, Loki::LongevityLifetime::DieLast, Loki::ClassLevelLockable, Loki::Mutex> 
			Loki::SingletonHolder<ConnectionPool, Loki::CreateUsingNew, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
			ConnectionPoolSingleton;

	} // !ADO
} // !Earlgrey