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
            vcProject.Load();

            Assert.IsNotNull(vcProject.Summary);
            Assert.AreEqual(vcProject.Summary, earlgreyProject);

            Assert.IsNotNull(vcProject.Details);
            Assert.AreEqual(2, vcProject.Details.Platforms.Count); // WIN32, x64
            Assert.AreEqual(4, vcProject.Details.Configurations.Count);
        }


        [Ignore]
        [Test]
        public void CopyPlatformConfiguration()
        {
            Project earlgreyProject = GetEarlgreyProject();

            var vcProject = new VcProject(earlgreyProject);
            vcProject.Load();

            string srcConfigurationPlatformName = "Debug|WIn32";

            Assert.IsTrue(
                vcProject.HasConfigurationPlatform(srcConfigurationPlatformName)
                );

            

            vcProject.Save();
        }
    }
}
