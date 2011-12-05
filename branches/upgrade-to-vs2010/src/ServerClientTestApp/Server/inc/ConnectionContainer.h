#pragma once

namespace Earlgrey {

class Connection;

}

class ConnectionContainer
{
public:
	typedef Earlgrey::xmap<LONG, std::tr1::shared_ptr<Earlgrey::Connection>>::Type ConnectionMapType;

	ConnectionContainer(void);
	~ConnectionContainer(void);

	LONG Add(std::tr1::shared_ptr<Earlgrey::Connection> connection)
	{
		Earlgrey::ScopedLock<> lock( _Lock );
		LONG Id = InterlockedIncrement( &_Id );
		_Connections.insert( std::make_pair( Id, connection ) );
		return Id;
	}

	void Remove(LONG Id)
	{
		Earlgrey::ScopedLock<> lock( _Lock );
		_Connections.erase( Id );
	}

private:
	Loki::Mutex _Lock;
	ConnectionMapType _Connections;
	volatile LONG _Id;
};


typedef Loki::SingletonHolder<ConnectionContainer> ConnectionsSingleton;