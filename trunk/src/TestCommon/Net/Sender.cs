using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon.Net
{
    public class Sender : IActor
    {
        public void HandleEvent(TcpSocket tcpSocket)
        {

        }

        public void HandleError(TcpSocket tcpSocket, int ErrorCode, string ErrorMessage)
        {

        }
    }
}
