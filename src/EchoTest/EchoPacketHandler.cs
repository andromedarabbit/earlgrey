using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using TestCommon;
using TestCommon.Net;

namespace EchoText
{
    [PacketHandler]
    public class EchoPacketHandler
    {
        const uint P_ECHO = 100;

        [Protocol(P_ECHO)]
        public void EchoProtocol(ProtocolParameter parameter)
        {
            InputStream IStream = new InputStream(parameter.Buffer);
            EchoProtocolPacket packet = new EchoProtocolPacket();
            IStream.Read(packet);
            Reporter.Log(ReportType.Normal, "ECHO : {0}", packet.Text);
        }
    }
}
