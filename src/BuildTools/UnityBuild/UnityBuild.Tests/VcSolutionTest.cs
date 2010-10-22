using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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

        [Test]
        public void CopyPlatformConfiguration()
        {
            var vcSolution = new VcSolution(AbstractTest.SolutionFilePath);
            vcSolution.Load();

            string srcPlatformConfigurationName = "Debug|Win32";
            // string srcPlatformConfigurationValue = "Debug|Win32";

            var configurationPlatforms = vcSolution.ConfigurationPlatforms;
            Assert.IsTrue(
                vcSolution.HasConfigurationPlatform(srcPlatformConfigurationName)
                );


            string dstPlatformConfigurationName = "Debug-UnityBuild|Win32";
            string dstPlatformConfigurationValue = "Debug-UnityBuild|Win32";

            Assert.IsFalse(
                vcSolution.HasConfigurationPlatform(dstPlatformConfigurationName)
                );

            var srcPlatformConfiguration = configurationPlatforms[srcPlatformConfigurationName];
            var dstPlatformConfiguration = new PropertyLine(dstPlatformConfigurationName, dstPlatformConfigurationValue);

            configurationPlatforms.Add(dstPlatformConfiguration);

            vcSolution.Save();
        }



    }
}

