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
    public class VcProjectTest // : AbstractTest
    {
        private static readonly string SolutionFilePath;

        // SolutionReaderTest 와 동일하다.
        static VcProjectTest()
        {
            SolutionFilePath = Path.Combine(
                AppDomain.CurrentDomain.BaseDirectory
                , @"..\..\..\Earlgrey.sln"
            );
            SolutionFilePath = Path.GetFullPath(SolutionFilePath);

        }

        [Test]
        public void TT()
        {
            SolutionFile slnFile = SolutionFile.FromFile(SolutionFilePath);

            var result = from project in slnFile.Projects
                               where project.ProjectName == "Earlgrey"
                               select project
                ;

            var earlgreyProject = result.First();

            var vcProject = new VcProject(earlgreyProject);
            vcProject.Run();

        }
    }
}
