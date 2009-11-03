using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Net.Sockets;

namespace TestCommon.Net
{
    public class PacketBuffer
    {
        /// <summary>
        /// Cannot use over-sized packet.
        /// </summary>
        public const int MaxBufferSize = 2048;

        private byte[] _Buffer = null;

        public byte[] Buffer
        {
            get { return _Buffer; }
        }

        public int UsedBufferLength
        {
            get;
            private set;
        }

        public int ReadOffset
        {
            get;
            private set;
        }

        public bool IsReadOnly
        {
            get;
            private set;
        }

        public PacketBuffer(byte[] Buffer, int UsedSize)
        {
            Debug.Assert(MaxBufferSize <= UsedSize);
            _Buffer = Buffer;
            UsedBufferLength = UsedSize;
            IsReadOnly = true;
        }

        public PacketBuffer()
        {
            _Buffer = new byte[MaxBufferSize];
            IsReadOnly = false;
        }

        public bool Append(byte[] Value, int Length)
        {
            return Append(Value, 0, Length);
        }

        public bool Append(byte[] Value, int Offset, int Length)
        {
            Debug.Assert(IsReadOnly == false);
            Debug.Assert(CanWrite(Length));
            
            if (UsedBufferLength + Length > MaxBufferSize)
            {
                Reporter.Log(ReportType.System, "Packet is too big.");
                return false;
            }

            Debug.Assert(Value.Length >= Length + Offset);

            if (Value.Length < Length + Offset)
            {
                Reporter.Log(ReportType.System, "Data buffer is shorter than 'Length'");
                return false;
            }

            Array.Copy(Value, Offset, _Buffer, UsedBufferLength, Length);
            UsedBufferLength += Length;

            return true;
        }

        public bool Read(byte[] Value, int Length)
        {
            Debug.Assert(CanRead(Length));

            if (!CanRead(Length))
            {
                Reporter.Log(ReportType.System, "There isn't enough data to read");
                return false;
            }

            Debug.Assert(Value.Length >= Length);

            if (Value.Length < Length)
            {
                Reporter.Log(ReportType.System, "Data buffer is shorter than 'Length'");
                return false;
            }

            Array.Copy(_Buffer, ReadOffset, Value, 0, Length);
            ReadOffset += Length;

            return true;
        }

        public bool CanWrite(int Length)
        {
            return UsedBufferLength + Length <= MaxBufferSize;
        }

        public bool CanRead(int Length)
        {
            return UsedBufferLength - ReadOffset >= Length;
        }
    }
}
