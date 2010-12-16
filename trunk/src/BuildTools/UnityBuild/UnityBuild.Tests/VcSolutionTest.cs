﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Earlgrey;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;
using MSBuild.Earlgrey.Tasks;
using MSBuild.Earlgrey.Tasks.Subversion;
using CWDev.SLNTools.Core;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    using UnityBuild;

    [TestFixture]
    public class VcSolutionTest : AbstractTest
    {
        [Test]
        public void LoadTest()
        {
            var vcSolution = new VcSolution(AbstractTest.SolutionFilePath);
            vcSolution.Load();
        }

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
        public void CreateNewSolutionConfigurationPlatform()
        {
            var vcSolution = new VcSolution(AbstractTest.SolutionFilePath);
            vcSolution.Load();

            const string dstSolutionConfigurationPlatformName = "Debug-UnityBuild|Win32";
            const string dstSolutionConfigurationPlatformValue = "Debug-UnityBuild|Win32";

            AddNewSolutionConfigurationPlatform(vcSolution, dstSolutionConfigurationPlatformName, dstSolutionConfigurationPlatformValue);

            vcSolution.Save();
        }

        public class ProjectConfigurationNameConverter : AbstractProjectConfigurationNameConverter
        {
            public override string GetNewName(string name)
            {
                return name + "-UnityBuild";
            }
        }

        public class SolutionConfigurationNameConverter : AbstractSolutionConfigurationNameConverter
        {
            public override string GetNewName(string name)
            {
                return name + "-UnityBuild";
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
        public void CopySolutionConfigurationPlatforms()
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
            var vcSolution = new VcSolution(AbstractTest.SolutionFilePath);
            vcSolution.Load();

            var solutionConverter = new SolutionConfigurationNameConverter();
            var projectConverter = new ProjectConfigurationNameConverter();

            vcSolution.CopySolutaionConfigurationPlatform(
                srcSolutionConfigurationName
                , srcSolutionPlatformName
                , solutionConverter
                , projectConverter
                );

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

    }
}

