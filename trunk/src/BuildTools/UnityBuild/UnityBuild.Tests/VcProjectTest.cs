using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class VcProjectTest : AbstractTest
    {
        [Test]
        public void ReadProjectDetails()
        {
            Project earlgreyProject = GetEarlgreyProject();

            var vcProject = new VcProject(earlgreyProject);
            vcProject.Initialize();

            Assert.IsNotNull(vcProject.Summary);
            Assert.AreEqual(vcProject.Summary, earlgreyProject);

            Assert.IsNotNull(vcProject.Details);
            Assert.AreEqual(2, vcProject.Details.Platforms.Length); // WIN32, x64
            Assert.AreEqual(4, vcProject.Details.Configurations.Length);
        }

      

    }
}
