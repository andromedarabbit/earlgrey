﻿using System;
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

    }
}

