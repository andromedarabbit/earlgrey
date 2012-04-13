using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    public class VcSolutionCopyTest : AbstractTest
    {
        [Test]
        public void CopySolutionConfigurationPlatform()
        {
            const string srcSolutionConfigurationName = "Debug";
            const string srcSolutionPlatformName = "Win32";

            const string dstConfigurationPlatformName = "Debug-UnityBuild|Win32";

			CopySolutionConfigurationPlatform(srcSolutionConfigurationName, srcSolutionPlatformName);

            VcProject vcProject = GetEarlgreyVcProject();

            IFileType throwErrorCpp = FindFile(vcProject, "ThrowError.cpp");
            Assert.IsTrue(throwErrorCpp.ExcludedFromBuild(dstConfigurationPlatformName));
        }

        [Test]
        public void CopySpecificSolutionConfigurationPlatforms()
        {
            string[] srcSolutionConfigurationNames = new string[] { "Debug", "Release" };
            string[] srcSolutionPlatformNames = new string[] { "Win32", "x64" };

            foreach (string configurationName in srcSolutionConfigurationNames)
            {
                foreach (string platformName in srcSolutionPlatformNames)
                {
					CopySolutionConfigurationPlatform(configurationName, platformName);
                }
            }
        }

		private void CopySolutionConfigurationPlatform(string srcSolutionConfigurationName, string srcSolutionPlatformName)
		{
			CopySolutionConfigurationPlatform(SolutionFilePath, srcSolutionConfigurationName, srcSolutionPlatformName);
		}

		private static void CopySolutionConfigurationPlatform(string solutionFilePath, string srcSolutionConfigurationName, string srcSolutionPlatformName)
        {
            var vcSolution = new VcSolution(solutionFilePath);
            vcSolution.Load();

            VcSolutionCopy copy = new VcSolutionCopy(vcSolution);

            copy.CopySolutionConfigurationPlatform(
                srcSolutionConfigurationName
                , srcSolutionPlatformName
                , true
                );

            vcSolution.Save();
        }

        [Test]
        public void CopySolutionConfigurationPlatforms()
        {
            var vcSolution = new VcSolution(SolutionFilePath);
            vcSolution.Load();

            VcSolutionCopy copy = new VcSolutionCopy(vcSolution);

            copy.CopySolutionConfigurationPlatform();

            vcSolution.Save();
        }

        [Test]
        public void CopySolutionConfigurationPlatformsWithExceptions()
        {
            var vcSolution = new VcSolution(SolutionFilePath);
            vcSolution.Load();

            VcSolutionCopy copy = new VcSolutionCopy(vcSolution);
            copy.ExcludeProject("gtest");
            copy.ExcludeProject("StackWalker_VC9");

            copy.CopySolutionConfigurationPlatform();

            vcSolution.Save();
        }

    }
}
