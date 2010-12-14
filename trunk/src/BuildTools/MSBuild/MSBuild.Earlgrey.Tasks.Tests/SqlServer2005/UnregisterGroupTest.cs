using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MSBuild.Earlgrey.Tasks.SqlServer2005;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.SqlServer2005
{
    [TestFixture]
    public class UnregisterServerTest
    {
        [Test]
        public void RequireName()
        {
            UnregisterServer instance = new UnregisterServer();
            instance.BuildEngine = new MockBuildEngine();

            Assert.IsFalse(instance.Execute());
        }


        [Test]
        public void UnregisterGroupRecursively()
        {
            const string groupName = "Group4";
            const string groupPath = "ServerGroup[@Name=''Local Instances'']/ServerGroup[@Name=''Group2'']";

            RegisterGroup registerInstance = new RegisterGroup();
            registerInstance.BuildEngine = new MockBuildEngine();
            registerInstance.Name = groupName;
            registerInstance.Path = groupPath;
            registerInstance.CreateRecursively = true;

            Assert.IsTrue(registerInstance.Execute());

            UnregisterGroup unregister = new UnregisterGroup();
            unregister.BuildEngine = new MockBuildEngine();
            unregister.Name = "Local Instances";
            unregister.Path = string.Empty;

            Assert.IsTrue(unregister.Execute());
        }

    }
}

