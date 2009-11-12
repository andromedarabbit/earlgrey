using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon.Net
{
    public interface IIDParser
    {
        uint GetID(PacketBuffer Buffer);
    }
}
