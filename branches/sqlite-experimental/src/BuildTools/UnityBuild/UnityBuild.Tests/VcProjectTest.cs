using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class VcProjectTest : AbstractTest
    {
        [Test]
        public void ReadProjectDetails()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            Assert.IsNotNull(vcProject.Summary);

            Assert.IsNotNull(vcProject.Details);
            Assert.AreEqual(2, vcProject.Details.Platforms.Count); // WIN32, x64
            Assert.AreEqual(4, vcProject.Details.Configurations.Count);
        }
        
        [Test]
        public void DeleteConfigurationPlatform()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            const string configurationPlatformName = "Debug|Win32";

            Assert.IsTrue(
                vcProject.HasConfigurationPlatform(configurationPlatformName)
                );
            Assert.GreaterOrEqual(EarlgreyProjectFileText.IndexOf(configurationPlatformName, StringComparison.CurrentCultureIgnoreCase), 0);

            vcProject.DeleteConfigurationPlatform(configurationPlatformName);

            vcProject.Save();

            Assert.IsFalse(
                 vcProject.HasConfigurationPlatform(configurationPlatformName)
                 );

            Assert.AreEqual(-1, EarlgreyProjectFileText.IndexOf(configurationPlatformName, StringComparison.CurrentCultureIgnoreCase));
        }

        [Test]
        public void GetPrecompiledHeaderOptionOfProjectEarlgrey()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            const string configurationPlatformName = "Release|x64";

            PrecompiledHeaderOptions option = vcProject.GetPrecompiledHeaderOption(configurationPlatformName);
            Assert.AreEqual(UsePrecompiledHeaderOptions.Use, option.UsePrecompiledHeader);
        }

        [Test]
        public void GetPrecompiledHeaderOption()
        {
            VcProject vcProject = GetVcProject("gtest");

            const string configurationPlatformName = "Debug|Win32";

            PrecompiledHeaderOptions option = vcProject.GetPrecompiledHeaderOption(configurationPlatformName);
            Assert.AreEqual(UsePrecompiledHeaderOptions.None, option.UsePrecompiledHeader);
        }

        [Test] 
        public void GetPrecompiledHeaderOptionOfFile()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            const string configurationPlatform = "Release|x64";
            
            FileType stdafxCpp = FindFile(vcProject, "stdafx.cpp");
            PrecompiledHeaderOptions stdafxCppOptions = vcProject.GetPrecompiledHeaderOption(configurationPlatform, stdafxCpp);
            Assert.AreEqual(
                UsePrecompiledHeaderOptions.Create, stdafxCppOptions.UsePrecompiledHeader
                );
            Assert.AreEqual("StdAfx.h", stdafxCppOptions.PrecompiledHeaderThrough);

            FileType stdafx2Cpp = FindFile(vcProject, "stdafx2.cpp");
            PrecompiledHeaderOptions stdafx2CppOptions = vcProject.GetPrecompiledHeaderOption(configurationPlatform, stdafx2Cpp);
            Assert.AreEqual(
                UsePrecompiledHeaderOptions.Create, stdafx2CppOptions.UsePrecompiledHeader
                );
            Assert.AreEqual("StdAfx2.h", stdafx2CppOptions.PrecompiledHeaderThrough);

            FileType throwErrorCpp = FindFile(vcProject, "throwError.cpp");
            PrecompiledHeaderOptions throwErrorCppOptions = vcProject.GetPrecompiledHeaderOption(configurationPlatform, throwErrorCpp);
            Assert.AreEqual(
                UsePrecompiledHeaderOptions.Use, throwErrorCppOptions.UsePrecompiledHeader
                );
            Assert.AreEqual("StdAfx.h", throwErrorCppOptions.PrecompiledHeaderThrough);

            FileType earlgreyH = FindFile(vcProject, "Earlgrey.h");
            PrecompiledHeaderOptions earlgreyHOptions = vcProject.GetPrecompiledHeaderOption(configurationPlatform, earlgreyH);
            Assert.AreEqual(
                UsePrecompiledHeaderOptions.None, earlgreyHOptions.UsePrecompiledHeader
                );
            Assert.IsTrue(string.IsNullOrEmpty(earlgreyHOptions.PrecompiledHeaderThrough));

            FileType noPrecompiledHeaderCpp = FindFile(vcProject, "NoPrecompiledHeader.cpp");
            PrecompiledHeaderOptions noPrecompiledHeaderCppOptions = vcProject.GetPrecompiledHeaderOption(configurationPlatform, noPrecompiledHeaderCpp);
            Assert.AreEqual(
               UsePrecompiledHeaderOptions.None, noPrecompiledHeaderCppOptions.UsePrecompiledHeader
               );
            Assert.IsTrue(string.IsNullOrEmpty(noPrecompiledHeaderCppOptions.PrecompiledHeaderThrough));
        }

        [Test]
        public void GetPrecompiledHeaders()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            const string configurationPlatform = "Debug|x64";

            IEnumerable<FileType> headers = vcProject.GetPrecompiledHeaders(configurationPlatform);
            Assert.AreEqual(2, headers.Count());


            FileType stdafxCpp = headers.First(item => item.FileName.Equals("stdafx.cpp", StringComparison.CurrentCultureIgnoreCase));
            Assert.IsNotNull(stdafxCpp);

            FileType stdafx2Cpp = headers.First(item => item.FileName.Equals("stdafx2.cpp", StringComparison.CurrentCultureIgnoreCase));
            Assert.IsNotNull(stdafx2Cpp);
        }
    }
}
