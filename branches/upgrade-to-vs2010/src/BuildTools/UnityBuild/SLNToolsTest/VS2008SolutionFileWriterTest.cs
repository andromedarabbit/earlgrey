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
    public class VS2008SolutionFileWriterTest : AbstractSolutionFileWriterTest
    {
        //private static readonly string SolutionFileFullPath;
        //private static readonly string NewSolutionFileFullPath;

        //static AbstractSolutionFileWriterTest()
        //{
        //    SolutionFileFullPath = Path.Combine(
        //        AppDomain.CurrentDomain.BaseDirectory
        //        , @"..\..\..\Earlgrey.sln"
        //    );
        //    SolutionFileFullPath = Path.GetFullPath(SolutionFileFullPath);

        //    NewSolutionFileFullPath = Path.Combine(
        //        AppDomain.CurrentDomain.BaseDirectory
        //        , @"..\..\..\NewEarlgrey.sln"
        //    );
        //    NewSolutionFileFullPath = Path.GetFullPath(NewSolutionFileFullPath);
        //}

        protected override string SolutionFileFullPath
        {
            get
            {
                string solutionFileFullPath = Path.Combine(
                    AppDomain.CurrentDomain.BaseDirectory
                        , @"..\..\UnityBuild\UnitTestSample\src\Earlgrey.sln"
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
                     , @"..\..\UnityBuild\UnitTestSample\src\NewEarlgrey.sln"
                );
                return Path.GetFullPath(NewSolutionFileFullPath);
            }
        }
    }
}
