using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MSBuild.Earlgrey.Tasks.Net;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.Net
{
    [TestFixture]
    public class WinScpTest
    {
        [Ignore]
        [Test]
        public void UsingFTPS()
        {
            // http://www.secureftp-test.com

            WinScp instance = new WinScp();
            instance.BuildEngine = new MockBuildEngine();
            instance.HostName = "ftp.secureftp-test.com";
            instance.ProtocolFtp = true;
            instance.Implicit = true;
            instance.UserName = "test"; 
            instance.Password = "test";
            // instance.HostKey = "ssh-rsa 2048 06:54:5d:6b:11:b3:2f:3f:fb:37:a7:2f:92:6f:41:d8:bb:40:2c:08";
            instance.HostKey = "ca:1f:10:c9:ec:33:cd:4c:1d:de:a2:93:54:02:5a:a5:c9:78:16:8e";
            instance.ScriptText = "pwd";
            instance.TimeoutSeconds = 20;
            Assert.IsTrue(instance.Execute());
        }

        [Ignore]
        [Test]
        public void UsingSFTP()
        {
            WinScp instance = new WinScp();
            WinScpTestHelper.Initialize(instance);

            instance.ScriptText = "pwd";
            Assert.IsTrue(instance.Execute());
        }
    }
}
