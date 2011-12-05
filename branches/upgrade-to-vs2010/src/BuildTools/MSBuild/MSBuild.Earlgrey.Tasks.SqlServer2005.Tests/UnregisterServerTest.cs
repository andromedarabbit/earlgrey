using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MSBuild.Earlgrey.Tasks.SqlServer2005;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005.Tests
{
    [TestFixture]
    public class UnregisterGroupTest
    {
        [Test]
        public void RequireName()
        {
            UnregisterGroup instance = new UnregisterGroup();
            instance.BuildEngine = new MockBuildEngine();

            Assert.IsFalse(instance.Execute());
        }
    }
}

