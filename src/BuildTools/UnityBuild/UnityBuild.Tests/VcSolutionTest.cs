using System;
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
    [TestFixture]
    public class VcSolutionTest // : AbstractTest
    {
        [Test]
        public void LoadTest()
        {
            var vcSolution = new VcSolution(AbstractTest.SolutionFilePath);
            vcSolution.Load();


        }

        [SetUp]
        public void SetUp()
        {
            Revert();
        }

        [TearDown]
        public void TearDown()
        {
            Revert();
        }

        private static void Revert()
        {
            SvnRevert cmd = new SvnRevert();
            cmd.BuildEngine = new MockBuildEngine();
        
            string path = Path.GetFullPath(
                Path.Combine(TaskUtility.ThisAssemblyDirectory, @"..\..\UnityBuild\UnitTestSample\src\")
                );

            cmd.Paths = new ITaskItem[1];
            cmd.Paths[0] = new TaskItem(path);

            cmd.Recursive = true;
            Assert.IsTrue(cmd.Execute());
        }

        [Test]
        public void CopyPlatformConfiguration()
        {
            var vcSolution = new VcSolution(AbstractTest.SolutionFilePath);
            vcSolution.Load();

            const string srcPlatformConfigurationName = "Debug|Win32";
            // string srcPlatformConfigurationValue = "Debug|Win32";

            var configurationPlatforms = vcSolution.ConfigurationPlatforms;
            Assert.IsTrue(
                vcSolution.HasConfigurationPlatform(srcPlatformConfigurationName)
                );


            const string dstPlatformConfigurationName = "Debug-UnityBuild|Win32";
            const string dstPlatformConfigurationValue = "Debug-UnityBuild|Win32";

            Assert.IsFalse(
                vcSolution.HasConfigurationPlatform(dstPlatformConfigurationName)
                );

            // var srcPlatformConfiguration = configurationPlatforms[srcPlatformConfigurationName];
            var dstPlatformConfiguration = new PropertyLine(dstPlatformConfigurationName, dstPlatformConfigurationValue);

            configurationPlatforms.Add(dstPlatformConfiguration);

            vcSolution.Save();
        }



    }
}

