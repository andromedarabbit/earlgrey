using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MSBuild.Earlgrey.Tasks.Net;
using MSBuild.Earlgrey.Tasks.Subversion;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.Net
{
    [TestFixture]
    public class WinScpTest
    {
        // [Ignore]
        [Test]
        public void UsingSFTP()
        {
            // http://www.secureftp-test.com

            WinScp instance = new WinScp();
            instance.BuildEngine = new MockBuildEngine();
            instance.HostName = "ftp.secureftp-test.com";
            instance.ProtocolSFtp = true;
            instance.Implicit = true;
            instance.UserName = "test"; 
            instance.Password = "test";
            // instance.HostKey = "ssh-rsa 2048 06:54:5d:6b:11:b3:2f:3f:fb:37:a7:2f:92:6f:41:d8:bb:40:2c:08";
            instance.HostKey = "06:54:5d:6b:11:b3:2f:3f:fb:37:a7:2f:92:6f:41:d8:bb:40:2c:08";
            instance.ScriptText = "pwd";
            instance.TimeoutSeconds = 20;
            Assert.IsTrue(instance.Execute());
        }
    }
}
