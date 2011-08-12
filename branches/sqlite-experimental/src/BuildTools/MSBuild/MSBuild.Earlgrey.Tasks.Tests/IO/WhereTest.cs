using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MSBuild.Earlgrey.Tasks.IO;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.IO
{
    [TestFixture]
    public class WhereTest
    {
        [Test]
        public void FindCmd()
        {
            Where instance = new Where();
            instance.BuildEngine = new MockBuildEngine();
            instance.FileName = "cmd.exe";

            Assert.IsTrue(instance.Execute());

            CollectionAssert.IsNotEmpty(instance.PathsFound);
        }
    }
}
