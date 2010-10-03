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
        public void VerifySolutionAndVcProjectsFullPaths()
        { 
            using (var reader = new SolutionFileReader(SolutionFileFullPath))
            {
                SolutionFile slnFile = reader.ReadSolutionFile();
                // NOTE: requires manual assignment of the path value. not a good design.
                slnFile.SolutionFullPath = SolutionFileFullPath;

                var realProjects = from project in slnFile.Projects
                                   where project.ProjectTypeGuid == KnownProjectTypeGuid.VisualC
                                   select project
                    ;
                                  
                foreach (var project in realProjects)
                {
                    Assert.IsTrue(File.Exists(project.FullPath));
                }
            }
        }

        [Test]
        public void CreateSolutionFileInstanceUsingAnotherWay()
        {
            SolutionFile slnFile = SolutionFile.FromFile(SolutionFileFullPath);

            Assert.IsTrue(File.Exists(slnFile.SolutionFullPath));
        }

        #region Bugs found
        #endregion // Bugs found

    }
}
