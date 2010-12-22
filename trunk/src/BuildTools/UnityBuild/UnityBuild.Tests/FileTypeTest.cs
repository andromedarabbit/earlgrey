using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class FileTypeTest : AbstractTest
    {
        [Test]
        public void ExcludedFromBuild()
        {
            const string configurationPlatform = "Debug|Win32";
            VcProject vcProject = GetEarlgreyVcProject();

            FileType throwErrorCpp = GetThrowErrorCpp(vcProject);
            Assert.IsTrue(throwErrorCpp.ExcludedFromBuild(configurationPlatform));
        }       

        [Test]
        public void IncludeFromBuild()
        {
            const string configurationPlatform = "Debug|Win32";
            VcProject vcProject = GetEarlgreyVcProject();

            FileType throwErrorCpp = GetThrowErrorCpp(vcProject);
            Assert.IsTrue(throwErrorCpp.ExcludedFromBuild(configurationPlatform));

            throwErrorCpp.IncludeInBuild(configurationPlatform);
            Assert.IsFalse(throwErrorCpp.ExcludedFromBuild(configurationPlatform));

            throwErrorCpp.ExcludeFromBuild(configurationPlatform);
            Assert.IsTrue(throwErrorCpp.ExcludedFromBuild(configurationPlatform));
        }

        [Test]
        public void IsSrcFile()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            FileType throwErrorCpp = GetThrowErrorCpp(vcProject);
            Assert.IsTrue(throwErrorCpp.IsSrcFile);

            FileType earlgreyH = GetEarlgreyH(vcProject);
            Assert.IsFalse(earlgreyH.IsSrcFile);
        }

        private static FileType GetEarlgreyH(VcProject vcProject)
        {
            return FindFile(vcProject, "Earlgrey.h");
        }

        private static FileType GetThrowErrorCpp(VcProject vcProject)
        {
            return FindFile(vcProject, "ThrowError.cpp");
        }    

        [Test]
        public void GetPrecompiledHeaderOption()
        {
            const string configurationPlatform = "Debug|Win32";
            VcProject vcProject = GetEarlgreyVcProject();

            FileType stdafxCpp = FindFile(vcProject, "stdafx.cpp");
            PrecompiledHeaderOptions stdafxCppOptions = stdafxCpp.GetPrecompiledHeaderOption(configurationPlatform);
            Assert.AreEqual(UsePrecompiledHeaderOptions.Create, stdafxCppOptions.UsePrecompiledHeader);

            FileType stdafx2Cpp = FindFile(vcProject, "stdafx2.cpp");
            PrecompiledHeaderOptions stdafx2CppOptions = stdafx2Cpp.GetPrecompiledHeaderOption(configurationPlatform);
            Assert.AreEqual(UsePrecompiledHeaderOptions.Create, stdafx2CppOptions.UsePrecompiledHeader);

            // 미리 컴파일된 헤더의 적용 여부는 VcProject 가 반환하는 기본 값과 파일의 값을 대조해야만 정확히 판단 가능하다.
            FileType throwErrorCpp = GetThrowErrorCpp(vcProject);
            PrecompiledHeaderOptions throwErrorCppOptions = throwErrorCpp.GetPrecompiledHeaderOption(configurationPlatform);
            Assert.AreEqual(UsePrecompiledHeaderOptions.InheritFromProject, throwErrorCppOptions.UsePrecompiledHeader);

            FileType earlgreyH = GetEarlgreyH(vcProject);
            PrecompiledHeaderOptions earlgreyHOptions = earlgreyH.GetPrecompiledHeaderOption(configurationPlatform);
            Assert.AreEqual(UsePrecompiledHeaderOptions.None, earlgreyHOptions.UsePrecompiledHeader);
        }
    }
}
