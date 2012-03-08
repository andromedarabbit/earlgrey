using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class VcProjectCopyTest : AbstractTest
    {
        [Test]
        public void CopyPlatformConfiguration()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            const string srcConfigurationPlatformName = "Debug|Win32";
            const string dstConfigurationPlatformName = "Debug-UnityBuild|Win32";

            Assert.IsTrue(
                vcProject.HasConfigurationPlatform(srcConfigurationPlatformName)
                );
            Assert.AreEqual(-1, EarlgreyProjectFileText.IndexOf(dstConfigurationPlatformName, StringComparison.CurrentCultureIgnoreCase));

            VcProjectCopy projectCopy
                = new VcProjectCopy(vcProject, srcConfigurationPlatformName, dstConfigurationPlatformName);
            projectCopy.CopyConfigurationPlatform();

            vcProject.Save();

            Assert.GreaterOrEqual(EarlgreyProjectFileText.IndexOf(dstConfigurationPlatformName, StringComparison.CurrentCultureIgnoreCase), 0);

            IFileType throwErrorCpp = FindFile(vcProject, "ThrowError.cpp");
            Assert.IsTrue(throwErrorCpp.ExcludedFromBuild(srcConfigurationPlatformName));
            Assert.IsTrue(throwErrorCpp.ExcludedFromBuild(dstConfigurationPlatformName));
        }

        
    }
}
