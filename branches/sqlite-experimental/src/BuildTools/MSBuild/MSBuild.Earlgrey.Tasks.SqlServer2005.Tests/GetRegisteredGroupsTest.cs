using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;
using MSBuild.Earlgrey.Tasks.SqlServer2005;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005.Tests
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
