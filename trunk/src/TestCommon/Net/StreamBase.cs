using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon.Net
{
    public class StreamBase
    {
        protected PacketBuffer _Buffer = new PacketBuffer();
        protected Client _Client = null;

        public StreamBase(Client client)
        {
            _Client = client;
        }
    }
}
