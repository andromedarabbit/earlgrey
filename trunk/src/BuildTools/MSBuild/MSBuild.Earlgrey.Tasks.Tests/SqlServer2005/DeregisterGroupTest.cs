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
    public class DeregisterGroupTest
    {
        [Test]
        public void RequireName()
        {
            RegisterGroup instance = new RegisterGroup();
            instance.BuildEngine = new MockBuildEngine();

            Assert.IsFalse(instance.Execute());
        }

        [Test]
        public void RegisterGroupToRootPath()
        {
            RegisterGroup instance = new RegisterGroup();
            instance.BuildEngine = new MockBuildEngine();
            instance.Name = "RegisterGroupsTest.RegisterGroupToRootPath";

            Assert.IsFalse(instance.Execute());
        }

        [Test]
        public void RegisterGroupUnderParentGroup()
        {
            RegisterGroup instance = new RegisterGroup();
            instance.BuildEngine = new MockBuildEngine();
            // instance.Name = "Goupr3";
            instance.Name = "Group4";
            instance.Path = "ServerGroup[@Name=''Local Instances'']/ServerGroup[@Name=''Group2'']";
            
            Assert.IsFalse(instance.Execute());
        }

    }
}

