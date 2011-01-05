using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class ProjectFileNameConverterTest
    {

        [Test]
        public void GetNewNameOfVcProj()
        {
            const string projectFileName = "gtest.vcproj";
            ProjectFileNameConverter converter = new ProjectFileNameConverter();
            string newProjectFileName = converter.GetNewName(projectFileName);

            StringAssert.AreEqualIgnoringCase("gtest-UnityBuild.vcproj", newProjectFileName);
        }

        [Test]
        public void IsNewName()
        {
            ProjectFileNameConverter converter = new ProjectFileNameConverter();

            Assert.IsTrue(
                converter.IsNewName("gtest-UnityBuild.vcproj")
                );
            Assert.IsFalse(
                converter.IsNewName("gtest.vcproj")
                );
        }

        [Test]
        [ExpectedException(typeof(ArgumentException))]
        public void IsNewNameUsingInvalidProjectName()
        {
            ProjectFileNameConverter converter = new ProjectFileNameConverter();

            Assert.IsTrue(
                converter.IsNewName("gtest-UnityBuild")
                );
        }

        [Test]
        [ExpectedException(typeof(ArgumentException))]
        public void IsNewNameUsingEmptyString()
        {
            ProjectFileNameConverter converter = new ProjectFileNameConverter();

            Assert.IsTrue(
                converter.IsNewName(string.Empty)
                );
        }
    }
}
