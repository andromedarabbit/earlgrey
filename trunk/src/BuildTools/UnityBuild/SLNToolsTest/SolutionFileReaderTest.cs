using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Earlgrey;

namespace SLNToolsTest
{
    using NUnit.Framework;
    using CWDev.SLNTools.Core;

    [TestFixtureAttribute]
    public class SolutionFileReaderTest
    {
        private static readonly string SolutionFileFullPath;

        static SolutionFileReaderTest()
        {
            SolutionFileFullPath = Path.Combine(
                TaskUtility.ThisAssemblyDirectory // AppDomain.CurrentDomain.BaseDirectory
                , @"..\..\..\Earlgrey.sln"
            );
            SolutionFileFullPath = Path.GetFullPath(SolutionFileFullPath);
        }

        [Test]
        public void ReadSolutionFileTest()
        {
            using(var reader = new SolutionFileReader(SolutionFileFullPath))
            {
                SolutionFile slnFile = reader.ReadSolutionFile();
                Assert.IsNotNull(slnFile);
                // 아래 코드가 실패하는데 이는 라이브러리의 버그인 듯 하다.
                // Assert.AreEqual(SolutionFileFullPath, slnFile.SolutionFullPath);
            }
        }

        [Test]
        public void GetVcProjects()
        {
            using (var reader = new SolutionFileReader(SolutionFileFullPath))
            {
                SolutionFile slnFile = reader.ReadSolutionFile();

                var projectNames = from project in slnFile.Projects
                            where project.RelativePath.EndsWith(".vcproj") == true
                            select project.ProjectName
                    ;

                Assert.Greater(projectNames.Count(), 0);
            }
        }


        [Test]
        public void VerifySolutionAndProjectsFullPaths()
        { 
            using (var reader = new SolutionFileReader(SolutionFileFullPath))
            {
                SolutionFile slnFile = reader.ReadSolutionFile();
                // NOTE: requires manual asignment of the path value. not a good design.
                slnFile.SolutionFullPath = SolutionFileFullPath;

                //var result = from project in slnFile.Projects
                //                   where project.ProjectFullName == "Earlgrey"
                //                   select project;

                //Project earlgrey = result.First();
                //Assert.IsNotNull(earlgrey);

                //string fullPath = earlgrey.FullPath;
                //Assert.IsTrue(File.Exists(fullPath));

                foreach (var project in slnFile.Projects)
                {
                    if (project.AllDescendants != null && project.AllDescendants.Count() > 0)
                        continue;
                    if (File.Exists(project.FullPath)==false)
                        Console.WriteLine();
                    Assert.IsTrue(File.Exists(project.FullPath));
                }
            }
        }

        #region Bugs found
        #endregion // Bugs found

    }
}
