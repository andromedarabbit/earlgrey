using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.SqlServer.Management.Smo;
using Microsoft.SqlServer.Management.Smo.RegisteredServers;
using MSBuild.Earlgrey.Tasks.SqlServer2005;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.SqlServer2005
{
    [TestFixture]
    public class GetRegisteredGroupsTest
    {
        [Test]
        public void Test()
        {
            GetRegisteredGroups instance = new GetRegisteredGroups();
            instance.BuildEngine = new MockBuildEngine();

            Assert.IsTrue(instance.Execute());

            ITaskItem[] names = instance.Names;
            Assert.IsNotNull(names); 
        }
    }
}
