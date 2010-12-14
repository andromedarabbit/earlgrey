using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MSBuild.Earlgrey.Tasks.SqlServer2005;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.SqlServer2005
{
    [TestFixture]
    public class RegisterGroupTest
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
            const string groupName = "RegisterGroupsTest.RegisterGroupToRootPath";
            
            RegisterGroup registerInstance = new RegisterGroup();
            registerInstance.BuildEngine = new MockBuildEngine();
            registerInstance.Name = groupName;

            Assert.IsTrue(registerInstance.Execute());

            UnregisterGroup unregister = new UnregisterGroup();
            unregister.BuildEngine = new MockBuildEngine();
            unregister.Name = groupName;

            Assert.IsTrue(unregister.Execute());
        }

        [Test]
        public void CreateRecursively()
        {
            const string groupName = "Group4";
            const string groupPath = "ServerGroup[@Name=''Local Instances'']/ServerGroup[@Name=''Group2'']";

            RegisterGroup registerInstance = new RegisterGroup();
            registerInstance.BuildEngine = new MockBuildEngine();
            registerInstance.Name = groupName;
            registerInstance.Path = groupPath;
            registerInstance.CreateRecursively = false;

            Assert.IsFalse(registerInstance.Execute());
        }

        [Test]
        public void RegisterGroupRecursively()
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
            unregister.Name = groupName;
            unregister.Path = groupPath;

            Assert.IsTrue(unregister.Execute());

            UnregisterGroup unregister2 = new UnregisterGroup();
            unregister2.BuildEngine = new MockBuildEngine();

            unregister2.Name = "Group2";
            unregister2.Path = "ServerGroup[@Name=''Local Instances'']";
            Assert.IsTrue(unregister2.Execute());

            UnregisterGroup unregister3 = new UnregisterGroup();
            unregister3.BuildEngine = new MockBuildEngine();

            unregister3.Name = "Local Instances";
            unregister3.Path = "";
            Assert.IsTrue(unregister3.Execute());
        }

    }
}

