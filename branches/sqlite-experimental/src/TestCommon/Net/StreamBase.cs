using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon.Net
{
    public class StreamBase
    {
        public StreamBase(PacketBuffer Buffer)
        {
            _Buffer = Buffer;
        }

        protected PacketBuffer _Buffer = null;
    }
}
