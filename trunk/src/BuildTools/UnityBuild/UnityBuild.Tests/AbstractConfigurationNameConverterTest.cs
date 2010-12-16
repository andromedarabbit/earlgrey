using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class AbstractConfigurationNameConverterTest
    {
        [Test]
        public void GetConfigurationPlatform()
        {
            const string configuration = "Debug";
            const string platform = "Win32";
            const string expected = "Debug|Win32";

            string result = AbstractConfigurationNameConverter.GetConfigurationPlatform(configuration, platform);
            Assert.AreEqual(expected, result);
        }

        [Test]
        public void GetConfigurationPlatformWithProperty()
        {
            const string configuration = "Release";
            const string platform = "x64";
            const string property = "ActiveCfg";
            const string expected = "Release|x64.ActiveCfg";

            string result = AbstractConfigurationNameConverter.GetConfigurationPlatform(configuration, platform, property);
            Assert.AreEqual(expected, result);
        }

        [Test]
        public void SplitConfigurationPlatform()
        {
            const string configurationPlatformName = "Debug|x64";

            string platformName;
            string projectConfigurationName; 
            string property;
            

            AbstractConfigurationNameConverter.SplitConfigurationPlatform(
                configurationPlatformName, out projectConfigurationName, out platformName, out property
                );

            Assert.AreEqual("x64", platformName);
            Assert.AreEqual("Debug", projectConfigurationName);
            Assert.IsTrue(string.IsNullOrEmpty(property));
        }

        [Test]
        public void SplitConfigurationPlatformWithProperty()
        {
            const string configurationPlatformName = "Debug|x64.Build.0";

            string platformName;
            string projectConfigurationName;
            string property;


            AbstractConfigurationNameConverter.SplitConfigurationPlatform(
                configurationPlatformName, out projectConfigurationName, out platformName, out property
                );

            Assert.AreEqual("x64", platformName);
            Assert.AreEqual("Debug", projectConfigurationName);
            Assert.AreEqual("Build.0", property);
        }
    }
}
