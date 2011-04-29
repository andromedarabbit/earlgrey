using System;
using System.IO;
using System.Linq;
using Earlgrey;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests
{
    [TestFixture]
    public class BetterMessageTest
    {
 
        [Test]
        public void Print()
        {
            string localPath = Path.GetFullPath(
                Path.Combine(TaskUtility.ThisAssemblyDirectory, @"..\..\")
                );

            BetterMessage diff = new BetterMessage();
            diff.BuildEngine = new MockBuildEngine();
            diff.Text = "This is a test.";            
            Assert.IsTrue(diff.Execute());
        }

       
    }
}
