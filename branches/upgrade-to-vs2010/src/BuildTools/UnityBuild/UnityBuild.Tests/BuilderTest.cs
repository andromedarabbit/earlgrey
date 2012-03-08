using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class BuilderTest : AbstractTest
    {
        [Test]
        public void Run()
        {
            using (Builder builder = new Builder(SolutionFilePath))
            {
                // TODO: 임시 주석
                //builder.ExcludeProject("gtest");
                //builder.ExcludeProject("StackWalker_VC9");

                builder.Run();
                builder.Dispose();
            }

            string[] dstConfigurationPlatformNames
                = new string[]
                      {
                          "Debug-UnityBuild|Win32", "Release-UnityBuild|Win32", "Debug-UnityBuild|x64", "Release-UnityBuild|x64"
                      }
                ;

            VcProject vcProject = GetEarlgreyVcProject();

            IFileType throwErrorCpp = FindFile(vcProject, "ThrowError.cpp");
            foreach (var dstConfigurationPlatformName in dstConfigurationPlatformNames)
            {
                Assert.IsTrue(throwErrorCpp.ExcludedFromBuild(dstConfigurationPlatformName));
            }


        }
    }
}
