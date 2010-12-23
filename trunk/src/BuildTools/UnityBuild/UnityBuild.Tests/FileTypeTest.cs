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

            FileType noPrecompiledHeaderCpp = FindFile(vcProject, "NoPrecompiledHeader.cpp");
            PrecompiledHeaderOptions noPrecompiledHeaderCppOptions = noPrecompiledHeaderCpp.GetPrecompiledHeaderOption(configurationPlatform);
            Assert.AreEqual(UsePrecompiledHeaderOptions.None, noPrecompiledHeaderCppOptions.UsePrecompiledHeader);
            
        }

        [Test]
        public void SetPrecompiledHeaderOptionOfStdAfxCpp()
        {
            const string configurationPlatform = "Release|Win32";
            VcProject vcProject = GetEarlgreyVcProject();

            // 변경 전
            FileType cpp = FindFile(vcProject, "stdafx.cpp");
            PrecompiledHeaderOptions options = cpp.GetPrecompiledHeaderOption(configurationPlatform);
            Assert.AreEqual(UsePrecompiledHeaderOptions.Create, options.UsePrecompiledHeader);
            Assert.AreEqual(string.Empty, options.PrecompiledHeaderThrough); // 파일에선 기본 값 상속시 빈 문자열을 가짐

            // 변경 
            PrecompiledHeaderOptions newOptions = new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.Use);
            newOptions.PrecompiledHeaderThrough = "stdafx2.h";
            newOptions.PrecompiledHeaderFile = options.PrecompiledHeaderFile;
            cpp.SetPrecompiledHeaderOption(configurationPlatform, newOptions);

            // 변경 후
            options = cpp.GetPrecompiledHeaderOption(configurationPlatform);
            Assert.AreEqual(UsePrecompiledHeaderOptions.Use, options.UsePrecompiledHeader);
            Assert.AreEqual("stdafx2.h", options.PrecompiledHeaderThrough);
        }

        [Test]
        public void SetPrecompiledHeaderOptionOfNoPreCompiledHeaderH()
        {
            const string configurationPlatform = "Release|Win32";
            VcProject vcProject = GetEarlgreyVcProject();

            // 변경 전
            FileType cpp = FindFile(vcProject, "NoPrecompiledHeader.cpp");
            PrecompiledHeaderOptions options = cpp.GetPrecompiledHeaderOption(configurationPlatform);
            Assert.AreEqual(UsePrecompiledHeaderOptions.None, options.UsePrecompiledHeader);
            Assert.AreEqual(string.Empty, options.PrecompiledHeaderThrough);

            // 변경 
            PrecompiledHeaderOptions newOptions = new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.Create);
            newOptions.PrecompiledHeaderThrough = "stdafx.h";
            newOptions.PrecompiledHeaderFile = options.PrecompiledHeaderFile;
            cpp.SetPrecompiledHeaderOption(configurationPlatform, newOptions);

            // 변경 후
            options = cpp.GetPrecompiledHeaderOption(configurationPlatform);
            Assert.AreEqual(UsePrecompiledHeaderOptions.Create, options.UsePrecompiledHeader);
            Assert.AreEqual("stdafx.h", options.PrecompiledHeaderThrough);
        }
    }
}
