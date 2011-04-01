using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MSBuild.Earlgrey.Tasks.SqlServer2005;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005.Tests
{
    [TestFixture]
    public class RegisterServerTest
    {
        [Test]
        public void RequireName()
        {
            RegisterServer instance = new RegisterServer();
            instance.BuildEngine = new MockBuildEngine();

            Assert.IsFalse(instance.Execute());
        }

        [Test]
        public void RegisterServerToRootPath()
        {
            const string serverName = "RegisterServersTest.RegisterServerToRootPath";
            
            RegisterServer registerInstance = new RegisterServer();
            registerInstance.BuildEngine = new MockBuildEngine();
            registerInstance.Name = serverName;
            registerInstance.ServerInstance = @".\SQLEXPRESS";

            Assert.IsTrue(registerInstance.Execute());

            UnregisterServer unregister = new UnregisterServer();
            unregister.BuildEngine = new MockBuildEngine();
            unregister.Name = serverName;

            Assert.IsTrue(unregister.Execute());

            
        }

        [Test]
        public void RegisterServerRecursively()
        {
            const string serverName = "UnitTest-RegisterServerUnderParentGroup";
            const string groupPath = "ServerGroup[@Name=''Local Instances'']/ServerGroup[@Name=''Group2'']";

            UnregisterLocalInstancesGroup();
            
            RegisterServer registerInstance = new RegisterServer();
            registerInstance.BuildEngine = new MockBuildEngine();
            registerInstance.Name = serverName;
            registerInstance.Path = groupPath;
            registerInstance.ServerInstance = @".\SQLEXPRESS";
            registerInstance.CreateRecursively = true;
                       

            Assert.IsTrue(registerInstance.Execute());
            
            UnregisterServer unregister = new UnregisterServer();
            unregister.BuildEngine = new MockBuildEngine();
            unregister.Name = serverName;
            unregister.Path = groupPath;
           
            Assert.IsTrue(unregister.Execute());

            UnregisterLocalInstancesGroup();
        }

        private static void UnregisterLocalInstancesGroup()
        {
            UnregisterGroup unregisterGroup = new UnregisterGroup();
            unregisterGroup.BuildEngine = new MockBuildEngine();
            unregisterGroup.Name = "Local Instances";
            unregisterGroup.Path = "";

            Assert.IsTrue(unregisterGroup.Execute());
        }
    }
}

