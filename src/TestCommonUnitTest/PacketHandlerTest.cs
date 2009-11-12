using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;
using TestCommon.Net;

namespace TestCommonUnitTest
{
    [TestFixture]
    public class PacketHandlerTest
    {
        [Test]
        public void PacketHandlerRegisterTest()
        {
            PacketHandlerManager.RegisterHandlers();
            Assert.IsTrue(PacketHandlerManager.Instance.IsExist(1000000));

            OutputStream oStream = new OutputStream();
            oStream.Write(1000000); // SamplePacketHandler.TestProtocol
            oStream.Write("Test");

            PacketBuffer Buffer = new PacketBuffer(oStream.Buffer.Buffer, oStream.Buffer.UsedBufferLength);
            PacketHandlerManager.Instance.Handle(new TcpSocket(), Buffer);
        }
    }
}
