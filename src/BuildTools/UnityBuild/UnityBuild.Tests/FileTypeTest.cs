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
        public void CreatePrecompiledHeader()
        {
            const string configurationPlatform = "Debug|Win32";
            VcProject vcProject = GetEarlgreyVcProject();

            FileType stdafxCpp = FindFile(vcProject, "stdafx.cpp");
            Assert.IsTrue(stdafxCpp.CreatePrecompiledHeader(configurationPlatform));

            FileType throwErrorCpp = GetThrowErrorCpp(vcProject);
            Assert.IsFalse(throwErrorCpp.CreatePrecompiledHeader(configurationPlatform));

            FileType earlgreyH = GetEarlgreyH(vcProject);
            Assert.IsFalse(earlgreyH.CreatePrecompiledHeader(configurationPlatform));
        }
    }
}
