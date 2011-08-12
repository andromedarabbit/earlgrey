using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using TestCommon;
using TestCommon.Net;

namespace EchoTest
{
    public class EchoProtocolPacket : Readable, Writable
    {
        public bool Read(InputStream IStream)
        {
            return IStream.Read(out _Text);
        }

        public bool Write(OutputStream OStream)
        {
            return OStream.Write(_Text);
        }

        public string Text
        {
            get { return _Text; }
            set { _Text = value; }
        }

        private string _Text = "";
    }
}
