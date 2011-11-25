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
            string expectedPath = Environment.GetEnvironmentVariable("ComSpec");

            Where instance = new Where();
            instance.BuildEngine = new MockBuildEngine();
            instance.FileName = "cmd.exe";            

            Assert.IsTrue(instance.Execute());

            CollectionAssert.IsNotEmpty(instance.PathsFound);
            Assert.AreEqual(1, instance.PathsFound.Distinct().Count());
            
            string identity = instance.PathsFound[0].GetMetadata("Identity");
            Assert.AreEqual(expectedPath, identity);


            string toString = instance.PathsFound[0].ToString();
            Assert.AreEqual(expectedPath, toString);
        }
    }
}
