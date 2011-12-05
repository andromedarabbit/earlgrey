using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.SqlServer.Management.Smo.RegisteredServers;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005.Tests
{
    using MSBuild.Earlgrey.Tasks.SqlServer2005;

    [TestFixture]
    public class RegisteredServerHelperTest
    {
        [Test]
        public void FindRootGroup()
        {
            const string parentPath = ""; // Root 
            ServerGroup group = RegisteredServerHelper.FindParentGroup(parentPath);
            Assert.IsNull(group);
        }

        [Test]
        [ExpectedException(typeof(ApplicationException), ExpectedMessage = "Not Found!")]
        public void FindUnknownGroup()
        {
            const string parentPath = "ServerGroup[@Name=''Local Instances'']";
            ServerGroup group = RegisteredServerHelper.FindParentGroup(parentPath);
        }

        [Test]
        public void FindGroup()
        {
            const string groupName = "Group4";
            const string groupPath = "ServerGroup[@Name=''Local Instances'']/ServerGroup[@Name=''Group2'']";


            RegisterGroup registerInstance = new RegisterGroup();
            registerInstance.BuildEngine = new MockBuildEngine();
            registerInstance.Name = groupName;
            registerInstance.Path = groupPath;
            registerInstance.CreateRecursively = true;

            Assert.IsTrue(registerInstance.Execute());



            ServerGroup group = RegisteredServerHelper.FindGroup(groupPath, groupName);
            Assert.IsNotNull(group);
            Assert.AreEqual(groupName, group.Name);




            UnregisterGroup unregister = new UnregisterGroup();
            unregister.BuildEngine = new MockBuildEngine();
            unregister.Name = "Local Instances";
            unregister.Path = string.Empty;

            Assert.IsTrue(unregister.Execute());
        }

        [Test]
        public void GetLastChildGroupName1()
        {
            const string path = "ServerGroup[@Name=''Local Instances'']/ServerGroup[@Name=''Instance'']";
            string lastChildGroupName = RegisteredServerHelper.GetLastChildGroupName(
                path
                );

            Assert.AreEqual("Instance", lastChildGroupName);
        }

        [Test]
        public void GetLastChildGroupName2()
        {
            const string path = @"ServerGroup[@Name=''Local Instance'']";
            string lastChildGroupName = RegisteredServerHelper.GetLastChildGroupName(
                path
                );

            Assert.AreEqual("Local Instance", lastChildGroupName);
        }



        [Test]
        public void GetParentGroupName1()
        {
            const string path = "ServerGroup[@Name=''Local Instances'']/ServerGroup[@Name=''Instance'']";
            string parentGroupPath = RegisteredServerHelper.GetParentGroupPath(
                path
                );

            Assert.AreEqual("ServerGroup[@Name=''Local Instances'']", parentGroupPath);
        }

        [Test]
        public void GetParentGroupName2()
        {
            const string path = @"ServerGroup[@Name=''Local Instance'']";
            string parentGroupPath = RegisteredServerHelper.GetParentGroupPath(
                path
                );

            Assert.AreEqual(string.Empty, parentGroupPath);
        }
    }
}
