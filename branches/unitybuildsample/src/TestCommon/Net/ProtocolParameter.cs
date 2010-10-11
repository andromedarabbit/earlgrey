using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon.Net
{
    public class ProtocolParameter
    {
        public ProtocolParameter(TcpSocket socket, PacketBuffer Buffer)
        {
            this.Socket = socket;
            this.Buffer = Buffer;
        }

        public TcpSocket Socket
        {
            get;
            private set;
        }

        public PacketBuffer Buffer
        {
            get;
            private set;
        }
    }
}
