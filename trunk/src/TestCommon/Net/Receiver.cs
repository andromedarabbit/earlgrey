using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace TestCommon.Net
{
    enum ReadState
    {
        Header,
        Body
    }

    public class Receiver : IActor
    {
        private ReadState _ReadState = ReadState.Header;
        private int _DataSize = 0;

        public void Initialize()
        {

        }

        public void HandleEvent(TcpSocket tcpSocket)
        {
            if (_ReadState == ReadState.Header)
            {
                Debug.Assert(tcpSocket.ReadSize == sizeof(UInt32));
                _DataSize = (int)BitConverter.ToUInt32(tcpSocket.ReadBuffer, 0);
                tcpSocket.Read(_DataSize);
            }
            else
            {
                Debug.Assert(tcpSocket.ReadSize == _DataSize);
                PacketHandler.Instance().Handle(tcpSocket, new PacketBuffer(tcpSocket.ReadBuffer, tcpSocket.ReadSize));
            }
        }

        public void HandleError(TcpSocket tcpSocket, int ErrorCode, string ErrorMessage)
        {

        }
    }
}
