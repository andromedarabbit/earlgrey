using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon.Net
{
    public class DefaultIDParser : IIDParser
    {
        public uint GetID(PacketBuffer Buffer)
        {
            InputStream iStream = new InputStream(Buffer);
            UInt32 id = 0;
            iStream.Read(out id);
            return (uint)id;
        }
    }
}
