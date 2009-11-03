using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace TestCommon.Net
{
    public interface Readable
    {
        bool Read(InputStream stream);
    }

    public class InputStream : StreamBase
    {
        public InputStream(PacketBuffer Buffer) : base(Buffer)
        {
        }

        public bool Read(out Int16 Value)
        {
            Value = 0;
            byte[] buffer = new byte[sizeof(Int16)];
            if (!_Buffer.Read(buffer, sizeof(Int16)))
            {
                return false;
            }
            Value = BitConverter.ToInt16(buffer, 0);
            return true;
        }

        public bool Read(out Int32 Value)
        {
            Value = 0;
            byte[] buffer = new byte[sizeof(Int32)];
            if (!_Buffer.Read(buffer, sizeof(Int32)))
            {
                return false;
            }
            Value = BitConverter.ToInt32(buffer, 0);
            return true;
        }

        public bool Read(out Int64 Value)
        {
            Value = 0;
            byte[] buffer = new byte[sizeof(Int64)];
            if (!_Buffer.Read(buffer, sizeof(Int64)))
            {
                return false;
            }
            Value = BitConverter.ToInt64(buffer, 0);
            return true;
        }

        public bool Read(out UInt16 Value)
        {
            Value = 0;
            byte[] buffer = new byte[sizeof(UInt16)];
            if (!_Buffer.Read(buffer, sizeof(UInt16)))
            {
                return false;
            }
            Value = BitConverter.ToUInt16(buffer, 0);
            return true;
        }

        public bool Read(out UInt32 Value)
        {
            Value = 0;
            byte[] buffer = new byte[sizeof(UInt32)];
            if (!_Buffer.Read(buffer, sizeof(UInt32)))
            {
                return false;
            }
            Value = BitConverter.ToUInt32(buffer, 0);
            return true;
        }

        public bool Read(out UInt64 Value)
        {
            Value = 0;
            byte[] buffer = new byte[sizeof(UInt64)];
            if (!_Buffer.Read(buffer, sizeof(UInt64)))
            {
                return false;
            }
            Value = BitConverter.ToUInt64(buffer, 0);
            return true;
        }

        public bool Read(out Single Value)
        {
            Value = 0;
            byte[] buffer = new byte[sizeof(Single)];
            if (!_Buffer.Read(buffer, sizeof(Single)))
            {
                return false;
            }
            Value = BitConverter.ToSingle(buffer, 0);
            return true;
        }

        public bool Read(out Double Value)
        {
            Value = 0;
            byte[] buffer = new byte[sizeof(Double)];
            if (!_Buffer.Read(buffer, sizeof(Double)))
            {
                return false;
            }
            Value = BitConverter.ToDouble(buffer, 0);
            return true;
        }

        public bool Read(out Char Value)
        {
            Value = '\0';
            byte[] buffer = new byte[sizeof(Char)];
            if (!_Buffer.Read(buffer, sizeof(Char)))
            {
                return false;
            }
            Value = BitConverter.ToChar(buffer, 0);
            return true;
        }

        public bool Read(Readable Value)
        {
            return Value.Read(this);
        }
    }
}
