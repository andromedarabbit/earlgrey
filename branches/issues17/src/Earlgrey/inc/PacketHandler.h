#pragma once 

namespace Earlgrey
{
	class Connectionhandler;
	class NetworkBuffer;
	class AsyncResult;

	class PacketHandler
	{
	public:
		typedef DWORD PacketIdType;
		typedef BOOL (*PacketHandlerFunction)(class ConnectionHandler* Connection, NetworkBuffer* Buffer, DWORD Start, DWORD End);
		typedef struct PacketHandlerNode
		{
			PacketHandlerFunction _Handler;

			PacketHandlerNode()
				: _Handler(NULL)
			{
			}
		};

		explicit PacketHandler(DWORD PacketNodeNum) 
		{
			_PacketHandlerTableSize = PacketNodeNum;
			_PacketHandlerTable = new PacketHandlerNode[_PacketHandlerTableSize];
		};

		~PacketHandler() 
		{
			delete[] _PacketHandlerTable;
		}

		void AddPacketHandler(PacketIdType Id, PacketHandlerFunction Handler)
		{
			EARLGREY_ASSERT(Id < _PacketHandlerTableSize);
			if (Id < _PacketHandlerTableSize)
			{
				_PacketHandlerTable[Id]._Handler = Handler;
			}
		}
		
		BOOL HandlePacket(PacketIdType Id, ConnectionHandler* Connection, NetworkBuffer* Buffer, AsyncResult* Result);

	private:
		//xmap<PacketIdType, PacketHandlerFunction> PacketHandlerMap;//그냥 배열로?
		PacketHandlerNode* _PacketHandlerTable;
		DWORD _PacketHandlerTableSize;

	};
}