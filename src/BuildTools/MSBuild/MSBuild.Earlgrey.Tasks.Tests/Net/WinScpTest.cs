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
        [Ignore]
        [Test]
        public void UsingSFTP()
        {
            WinScp instance = new WinScp();
            instance.BuildEngine = new MockBuildEngine();
            instance.HostName = "andromedarabbit.net";
            instance.ProtocolSFtp = true;
            instance.Password = "";
            instance.UserName = "";
            instance.HostKey = "ssh-rsa 2048 a4:d0:ec:7a:32:33:4e:43:2d:d2:c9:e8:ca:d0:42:c0";
            instance.ScriptText = "cd /home";
            Assert.IsTrue(instance.Execute());
        }
    }
}
