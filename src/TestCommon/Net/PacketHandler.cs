using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace TestCommon.Net
{
    public class PacketHandler
    {
        public UInt32 HeaderSize
        {
            get { return sizeof(UInt32); }
        }
        public UInt32 BodySize
        {
            get;
            private set;
        }
        public void ParseHeader(byte[] Buffer, int Size)
        {
            Debug.Assert(HeaderSize == Size);
            BodySize = BitConverter.ToUInt32(Buffer, 0);
        }

        public bool Handle(byte[] Buffer, int Size)
        {
            return true;
        }
    }
}
