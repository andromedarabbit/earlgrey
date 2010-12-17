using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    public class VcSolutionCopyTest : AbstractTest
    {
        public override void SetUp()
        {
            base.SetUp();
            Revert();
        }

        public override void TearDown()
        {
            base.TearDown();
            Revert();
        }

        [Test]
        public void CopySolutionConfigurationPlatform()
        {
            const string srcSolutionConfigurationName = "Debug";
            const string srcSolutionPlatformName = "Win32";

            CopySolutionConfigurationPlatform(srcSolutionConfigurationName, srcSolutionPlatformName);
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

        private static void CopySolutionConfigurationPlatform(string srcSolutionConfigurationName, string srcSolutionPlatformName)
        {
            var vcSolution = new VcSolution(SolutionFilePath);
            vcSolution.Load();

            VcSolutionCopy copy = new VcSolutionCopy(vcSolution);

            var solutionConverter = new VcSolutionTest.SolutionConfigurationNameConverter();
            var projectConverter = new VcSolutionTest.ProjectConfigurationNameConverter();

            copy.CopySolutionConfigurationPlatform(
                srcSolutionConfigurationName
                , srcSolutionPlatformName
                , solutionConverter
                , projectConverter
                );

            vcSolution.Save();
        }

        [Test]
        public void CopySolutionConfigurationPlatforms()
        {
            var vcSolution = new VcSolution(SolutionFilePath);
            vcSolution.Load();

            VcSolutionCopy copy = new VcSolutionCopy(vcSolution);

            var solutionConverter = new VcSolutionTest.SolutionConfigurationNameConverter();
            var projectConverter = new VcSolutionTest.ProjectConfigurationNameConverter();

            copy.CopySolutionConfigurationPlatform(
                solutionConverter
                , projectConverter
                );

            vcSolution.Save();
        }

     
    }
}
