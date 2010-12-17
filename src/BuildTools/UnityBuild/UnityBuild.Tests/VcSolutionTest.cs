using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    using UnityBuild;

    [TestFixture]
    public class VcSolutionTest : AbstractTest
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
        public void LoadTest()
        {
            var vcSolution = new VcSolution(SolutionFilePath);
            vcSolution.Load();
        }

        [Test]
        public void GetConfigurationPlatforms()
        {
            var vcSolution = new VcSolution(SolutionFilePath);
            vcSolution.Load();

            IEnumerable<string> names = vcSolution.ConfigurationPlatformNames;
            Assert.AreEqual(8, names.Count());
        }

        [Test]
        public void CreateNewSolutionConfigurationPlatform()
        {
            var vcSolution = new VcSolution(SolutionFilePath);
            vcSolution.Load();

            const string dstSolutionConfigurationPlatformName = "Debug-UnityBuild|Win32";
            const string dstSolutionConfigurationPlatformValue = "Debug-UnityBuild|Win32";

            AddNewSolutionConfigurationPlatform(vcSolution, dstSolutionConfigurationPlatformName, dstSolutionConfigurationPlatformValue);

            vcSolution.Save();
        }

        private static void AddNewSolutionConfigurationPlatform(VcSolution vcSolution, string dstPlatformConfigurationName, string dstPlatformConfigurationValue)
        {
            const string srcPlatformConfigurationName = "Debug|Win32";
            // string srcPlatformConfigurationValue = "Debug|Win32";

            var configurationPlatforms = vcSolution.ConfigurationPlatforms;
            Assert.IsTrue(
                vcSolution.HasSolutionConfigurationPlatform(srcPlatformConfigurationName)
                );

            Assert.IsFalse(
                vcSolution.HasSolutionConfigurationPlatform(dstPlatformConfigurationName)
                );

            // var srcPlatformConfiguration = configurationPlatforms[srcPlatformConfigurationName];
            var dstPlatformConfiguration = new PropertyLine(dstPlatformConfigurationName, dstPlatformConfigurationValue);

            configurationPlatforms.Add(dstPlatformConfiguration);
        }

        public class ProjectConfigurationNameConverter : AbstractProjectConfigurationNameConverter
        {
            private const string Suffix = "-UnityBuild";

            public override string GetNewName(string name)
            {
                if(name == null)
                    throw new ArgumentNullException();

                if(name.Length == 0)
                    throw new ArgumentException();

                return name + Suffix;
            }

            public override string GetOldName(string name)
            {
                if(name.EndsWith(Suffix) == false)
                    throw new ArgumentException();

                return name + Suffix;
            }
        }

        public class SolutionConfigurationNameConverter : AbstractSolutionConfigurationNameConverter
        {
            private const string Suffix = "-UnityBuild";

            public override string GetNewName(string name)
            {
                if (name == null)
                    throw new ArgumentNullException();

                if (name.Length == 0)
                    throw new ArgumentException();

                return name + Suffix;
            }

            public override string GetOldName(string name)
            {
                if (name.EndsWith(Suffix) == false)
                    throw new ArgumentException();

                return name + Suffix;
            }
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
            string[] srcSolutionConfigurationNames = new string[] {"Debug", "Release"};
            string[] srcSolutionPlatformNames = new string[] {"Win32", "x64"};

            foreach(string configurationName in srcSolutionConfigurationNames)
            {
                foreach(string platformName in srcSolutionPlatformNames)
                {
                    CopySolutionConfigurationPlatform(configurationName, platformName);        
                }
            }
        }

        private static void CopySolutionConfigurationPlatform(string srcSolutionConfigurationName, string srcSolutionPlatformName)
        {
            var vcSolution = new VcSolution(SolutionFilePath);
            vcSolution.Load();

            var solutionConverter = new SolutionConfigurationNameConverter();
            var projectConverter = new ProjectConfigurationNameConverter();

            vcSolution.CopySolutionConfigurationPlatform(
                srcSolutionConfigurationName
                , srcSolutionPlatformName
                , solutionConverter
                , projectConverter
                );

            vcSolution.Save();
        }

        /*
        [Test]
        public void DeleteSolutionConfigurationPlatform()
        {
            const string solutionConfigurationName = "Debug";
            const string solutionPlatformName = "x64";

            var vcSolution = new VcSolution(SolutionFilePath);
            vcSolution.Load();

            vcSolution.DeleteSolutionConfigurationPlatform(solutionConfigurationName, solutionPlatformName);

            vcSolution.Save();
        }
        */

        [Test]
        public void CopySolutionConfigurationPlatforms()
        {
            var vcSolution = new VcSolution(SolutionFilePath);
            vcSolution.Load();

            var solutionConverter = new SolutionConfigurationNameConverter();
            var projectConverter = new ProjectConfigurationNameConverter();

            vcSolution.CopySolutionConfigurationPlatform(
                solutionConverter
                , projectConverter
                );

            vcSolution.Save();
        }
    }
}

