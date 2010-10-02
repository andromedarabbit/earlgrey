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

        static AbstractTest()
        {
            _ThisDir = Path.Combine(
                TaskUtility.ThisAssemblyDirectory
                , @"UnityBuildTestResources"
                );

            _TempDir = Path.Combine(_ThisDir, "Temp");
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
    }
}
