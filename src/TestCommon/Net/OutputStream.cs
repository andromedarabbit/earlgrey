using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon.Net
{
    public interface Writable
    {
        bool Write(OutputStream stream);
    }

    public class OutputStream : StreamBase
    {
        public OutputStream(Client client) : base(client)
        {
        }

        public bool Write(Writable Value)
        {
            return Value.Write(this);
        }

        public bool Write(Char Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(Char));
        }

        public bool Write(Double Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(Double));
        }

        public bool Write(Int16 Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(Int16));
        }

        public bool Write(Int32 Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(Int32));
        }

        public bool Write(Int64 Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(Int64));
        }

        public bool Write(Single Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(Single));
        }

        public bool Write(UInt16 Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(UInt16));
        }

        public bool Write(UInt32 Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(UInt16));
        }

        public bool Write(UInt64 Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(UInt16));
        }

        /// <summary>
        /// String packet consists of string length(uint32) and null-terminated string.
        /// </summary>
        /// <param name="Value">string value</param>
        /// <returns>success(bool)</returns>
        public bool Write(string Value)
        {
            byte[] EncodedString = Encoding.Default.GetBytes(Value.ToCharArray());

            if (!Write((UInt32)Value.Length))
            {
                return false;
            }
            return _Buffer.Append(EncodedString, EncodedString.Length);
        }

        public bool Flush()
        {
            if (!_Client.Send(_Buffer.Buffer, _Buffer.UsedBufferLength))
            {
                return false;
            }
            return true;
        }
    }
}
