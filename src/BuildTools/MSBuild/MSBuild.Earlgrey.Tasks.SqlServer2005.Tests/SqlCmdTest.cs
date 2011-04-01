using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MSBuild.Earlgrey.Tasks.SqlServer2005;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005.Tests
{
    [TestFixture]
    public class SqlCmdTest
    {
        [Test]
        public void Query()
        {
            SqlCmd instance = new SqlCmd();
            instance.BuildEngine = new MockBuildEngine();

            instance.Server = @"localhost\SQLEXPRESS";
            instance.DbName = "master";
            instance.Query = "SELECT * FROM INFORMATION_SCHEMA.TABLES;";

            Assert.IsTrue(instance.Execute());
        }

        [Test]
        public void NoQueryPrivided()
        {
            SqlCmd instance = new SqlCmd();
            instance.BuildEngine = new MockBuildEngine();

            instance.Server = @"localhost\SQLEXPRESS";

            Assert.IsTrue(instance.Execute());
        }
    }
}