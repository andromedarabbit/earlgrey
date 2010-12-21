using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class VcProjectTest : AbstractTest
    {
        [Test]
        public void ReadProjectDetails()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            Assert.IsNotNull(vcProject.Summary);

            Assert.IsNotNull(vcProject.Details);
            Assert.AreEqual(2, vcProject.Details.Platforms.Count); // WIN32, x64
            Assert.AreEqual(4, vcProject.Details.Configurations.Count);
        }

        [Test]
        public void CopyPlatformConfiguration()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            const string srcConfigurationPlatformName = "Debug|WIn32";
            const string dstConfigurationPlatformName = "Debug-UnityBuild|Win32";

            Assert.IsTrue(
                vcProject.HasConfigurationPlatform(srcConfigurationPlatformName)
                );
            Assert.AreEqual(-1, EarlgreyProjectFileText.IndexOf(dstConfigurationPlatformName, StringComparison.CurrentCultureIgnoreCase));

            vcProject.CopyConfigurationPlatform(srcConfigurationPlatformName, dstConfigurationPlatformName);

            vcProject.Save();

            Assert.GreaterOrEqual(EarlgreyProjectFileText.IndexOf(dstConfigurationPlatformName, StringComparison.CurrentCultureIgnoreCase), 0);
            
            FileType throwErrorCpp = FindFile(vcProject, "ThrowError.cpp");
            Assert.IsTrue(throwErrorCpp.ExcludedFromBuild(srcConfigurationPlatformName));
            Assert.IsTrue(throwErrorCpp.ExcludedFromBuild(dstConfigurationPlatformName));
        }
        
        [Test]
        public void DeleteConfigurationPlatform()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            const string configurationPlatformName = "Debug|WIn32";

            Assert.IsTrue(
                vcProject.HasConfigurationPlatform(configurationPlatformName)
                );
            Assert.GreaterOrEqual(EarlgreyProjectFileText.IndexOf(configurationPlatformName, StringComparison.CurrentCultureIgnoreCase), 0);

            vcProject.DeleteConfigurationPlatform(configurationPlatformName);

            vcProject.Save();

            Assert.IsFalse(
                 vcProject.HasConfigurationPlatform(configurationPlatformName)
                 );

            Assert.AreEqual(-1, EarlgreyProjectFileText.IndexOf(configurationPlatformName, StringComparison.CurrentCultureIgnoreCase));
        }
    }
}
