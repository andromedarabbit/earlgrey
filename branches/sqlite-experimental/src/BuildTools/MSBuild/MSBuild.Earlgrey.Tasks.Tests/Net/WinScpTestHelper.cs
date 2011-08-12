using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MSBuild.Earlgrey.Tasks.Net;

namespace MSBuild.Earlgrey.Tasks.Tests.Net
{
    internal static class WinScpTestHelper
    {
        public static void Initialize(AbstractWinScp instance)
        {
            instance.BuildEngine = new MockBuildEngine();
            instance.HostName = "";
            instance.ProtocolSFtp = true;

            instance.UserName = "";
            instance.Password = "";
            instance.HostKey = "";
            instance.TimeoutSeconds = 20;

        }
    }
}
