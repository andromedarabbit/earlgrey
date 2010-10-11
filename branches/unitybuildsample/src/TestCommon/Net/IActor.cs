using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon.Net
{
    public interface IActor
    {
        void HandleEvent(TcpSocket tcpSocket);
        void HandleError(TcpSocket tcpSocket, int ErrorCode, string ErrorMessage);
    }
}
