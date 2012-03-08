using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class SrcFileAppendTest : AbstractTest
    {
        [Test]
        public void ReadProjectDetails()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            // Assert.IsNotNull(vcProject.Summary);

            Assert.AreEqual(2, vcProject.Platforms.Count()); // WIN32, x64
            Assert.AreEqual(4, vcProject.Configurations.Count());
        }

    }
}
