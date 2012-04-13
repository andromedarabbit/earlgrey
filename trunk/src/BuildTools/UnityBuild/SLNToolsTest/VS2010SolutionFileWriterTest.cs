using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace SLNTools.Tests
{
    using NUnit.Framework;
    using CWDev.SLNTools.Core;

    [Ignore]
    [TestFixtureAttribute]
    public class VS2010SolutionFileWriterTest : AbstractSolutionFileWriterTest
    {
        protected override string SolutionFileFullPath
        {
            get
            {
                string solutionFileFullPath = Path.Combine(
                    AppDomain.CurrentDomain.BaseDirectory
                        , @"..\..\UnityBuild\UnitTestSample\src\Earlgrey.vs2010.sln"
                    );
                return Path.GetFullPath(SolutionFileFullPath);
            }
        }

        protected override string NewSolutionFileFullPath
        {
            get
            {
                string solutionFileFullPath = Path.GetFullPath(SolutionFileFullPath);

                solutionFileFullPath = Path.Combine(
                     AppDomain.CurrentDomain.BaseDirectory
                     , @"..\..\UnityBuild\UnitTestSample\src\NewEarlgrey.vs2010.sln"
                );
                return Path.GetFullPath(NewSolutionFileFullPath);
            }
        }
    }
}
