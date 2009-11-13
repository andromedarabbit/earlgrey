using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon.Net
{
    public class TestProtocolPacket : Readable
    {
        public bool Read(InputStream IStream)
        {
            return IStream.Read(out this._Text);
        }
        public string Text
        {
            get { return _Text; }
        }

        private string _Text = "";
    }

    [PacketHandler]
    public class SamplePacketHandler
    {
        [Protocol(1000000)]
        public void TestProtocol(ProtocolParameter parameter)
        {
            InputStream IStream = new InputStream(parameter.Buffer);
            TestProtocolPacket packet = new TestProtocolPacket();
            IStream.Read(packet);
        }
    }
}
