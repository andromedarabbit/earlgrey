﻿using System;
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


        [Test]
        public void CopyPlatformConfiguration()
        {
            Project earlgreyProject = GetEarlgreyProject();

            var vcProject = new VcProject(earlgreyProject);
            vcProject.Load();

            const string srcConfigurationPlatformName = "Debug|WIn32";
            const string dstConfigurationPlatformName = "Debug-UnityBuild|Win32";

            Assert.IsTrue(
                vcProject.HasConfigurationPlatform(srcConfigurationPlatformName)
                );

            
            vcProject.CopyConfigurationPlatform(srcConfigurationPlatformName, dstConfigurationPlatformName);

            vcProject.Save();
        }
    }
}