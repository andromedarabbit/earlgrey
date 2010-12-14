using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using Microsoft.SqlServer.Management.Smo.RegisteredServers;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.SqlServer2005
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
    }
}
