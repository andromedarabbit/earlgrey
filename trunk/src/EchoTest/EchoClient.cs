using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using TestCommon.Net;
using TestCommon;
using TestCommon.Components;

namespace EchoText
{
    [TestFixture("ECHO > ", "Echo Test")]
    public class EchoClient
    {
        TcpSocket socket = new TcpSocket();

        [SetUp]
        bool Initialize()
        {
            if (!socket.Connect(IPAddress.Loopback, 8000))
            {
                Reporter.Log(ReportType.Error, "Connection failed");
                return false;
            }

            return true;
        }

        [TearDown]
        void CleanUp()
        {
            socket.Close();
        }

        [Test(TestType.Interactive)]
        bool Echo(string test)
        {
            if (test == "quit")
            {
                Reporter.Log(ReportType.Normal, "Quit!");
                return false;
            }

            EchoProtocolPacket packet = new EchoProtocolPacket();
            packet.Text = test;
            OutputStream OStream = new OutputStream();
            if (!packet.Write(OStream))
            {
                Reporter.Log(ReportType.Error, "Writing failed.");
                return false;
            }

            if (!OStream.Flush(socket))
            {
                Reporter.Log(ReportType.Error, "Flushing failed.");
                return false;
            }
            return true;
        }
    }
}
