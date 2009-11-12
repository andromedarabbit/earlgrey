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
        public OutputStream() : base(new PacketBuffer())
        {
        }

        public PacketBuffer Buffer
        {
            get { return _Buffer; }
        }

        public bool Write(Writable Value)
        {
            return Value.Write(this);
        }

        public bool Write(char Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(Char));
        }

        public bool Write(double Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(double));
        }

        public bool Write(short Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(short));
        }

        public bool Write(int Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(int));
        }

        public bool Write(long Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(long));
        }

        public bool Write(float Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(float));
        }

        public bool Write(ushort Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(ushort));
        }

        public bool Write(uint Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(uint));
        }

        public bool Write(ulong Value)
        {
            return _Buffer.Append(BitConverter.GetBytes(Value), sizeof(ulong));
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

        public bool Flush(TcpSocket tcpSocket)
        {
            if (!tcpSocket.Send(_Buffer.Buffer, _Buffer.UsedBufferLength))
            {
                return false;
            }
            return true;
        }
    }
}
