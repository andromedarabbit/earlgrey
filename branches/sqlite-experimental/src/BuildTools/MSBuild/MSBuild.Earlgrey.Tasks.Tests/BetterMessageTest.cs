using System;
using System.IO;
using System.Linq;
using Earlgrey;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests
{
    [TestFixture]
    public class BetterMessageTest
    {
 
        [Test]
        public void Print()
        {
            BetterMessage diff = new BetterMessage();
            diff.BuildEngine = new MockBuildEngine();
            diff.Text = new ITaskItem[]
                            {
                                new TaskItem("This is a test.")
                                , new TaskItem("This is a test!")
                            };
            Assert.IsTrue(diff.Execute());
        }

       
    }
}
