using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Earlgrey;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    public class AbstractTest
    {
        private static readonly string _TempDir;
        private static readonly string _ThisDir;
        private static readonly string _SolutionFilePath;
        private static readonly string _SampleVcProjFilePath;

        static AbstractTest()
        {
            _ThisDir = Path.Combine(
                TaskUtility.ThisAssemblyDirectory
                , @"UnityBuildTestResources"
                );

            _TempDir = Path.Combine(_ThisDir, "Temp");

            _SolutionFilePath = Path.Combine(
                AppDomain.CurrentDomain.BaseDirectory
                , @"..\..\..\BuildTools\UnityBuild\UnitTestSample\src\Earlgrey.sln"
            );
            _SolutionFilePath = Path.GetFullPath(_SolutionFilePath);

            _SampleVcProjFilePath = Path.Combine(
               AppDomain.CurrentDomain.BaseDirectory
               , @"..\..\..\BuildTools\UnityBuild\UnitTestSample\src\Earlgrey\Earlgrey.vcproj"
           );
            _SampleVcProjFilePath = Path.GetFullPath(_SampleVcProjFilePath);
        }

        [SetUp]
        public virtual void SetUp()
        {
            if(Directory.Exists(_TempDir))
                Directory.Delete(_TempDir, true);

            Directory.CreateDirectory(_TempDir);
        }

        [TearDown]
        public virtual void TearDown()
        {
            if (Directory.Exists(_TempDir))
                Directory.Delete(_TempDir, true);
        }

        public static string TempDir
        {
            get { return _TempDir; }
        }

        public static string ThisDir
        {
            get { return _ThisDir; }
        }

        public static string SolutionFilePath
        {
            get { return _SolutionFilePath;  }
        }

        public static string SampleVcProjFilePath
        {
            get { return _SampleVcProjFilePath; }
        }

    }
}
