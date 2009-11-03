using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace TestCommon.Net
{
    public class PacketHandler
    {
        private static PacketHandler _PacketHandler = new PacketHandler();
        public static PacketHandler Instance()
        {
            return _PacketHandler;
        }

        public bool Handle(TcpSocket tcpSocket, PacketBuffer Buffer)
        {
            return true;
        }
    }
}
