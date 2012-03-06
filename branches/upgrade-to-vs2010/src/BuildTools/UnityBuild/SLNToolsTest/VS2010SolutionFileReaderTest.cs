using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Earlgrey;

namespace SLNTools.Tests
{
    using NUnit.Framework;
    using CWDev.SLNTools.Core;

    [TestFixtureAttribute]
    public class VS2010SolutionFileReaderTest : AbstractSolutionFileReaderTest
    {
        protected override string SolutionFileFullPath
        {
            get
            {
                string solutionFileFullPath = Path.Combine(
                    TaskUtility.ThisAssemblyDirectory // AppDomain.CurrentDomain.BaseDirectory
                    , @"..\..\UnityBuild\UnitTestSample\src\Earlgrey.vs2010.sln"
                );
                return Path.GetFullPath(solutionFileFullPath);
            }
        }

        protected override string VcProjectFileExtension
        {
            get { return ".vcxproj"; }
        }
    }
}
