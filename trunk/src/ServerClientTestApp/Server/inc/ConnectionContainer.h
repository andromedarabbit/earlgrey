#pragma once

namespace Earlgrey {

class Connection;

}

class ConnectionContainer
{
public:
	typedef std::set<std::tr1::shared_ptr<Earlgrey::Connection>> ConnectionSetType;

	ConnectionContainer(void);
	~ConnectionContainer(void);

	void Add(std::tr1::shared_ptr<Earlgrey::Connection> connection)
	{
		_Connections.insert( connection );
	}

	void Remove(std::tr1::shared_ptr<Earlgrey::Connection> connection)
	{
		_Connections.erase( connection );
	}

private:
	ConnectionSetType _Connections;
};


typedef Loki::SingletonHolder<ConnectionContainer> ConnectionsSingleton;