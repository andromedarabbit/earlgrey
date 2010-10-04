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
    public class VcProjectTest : AbstractTest
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
        public void ReadProjectDetails()
        {
            Project earlgreyProject = GetEarlgreyProject();

            var vcProject = new VcProject(earlgreyProject);
            vcProject.Initialize();

            Assert.IsNotNull(vcProject.Summary);
            Assert.AreEqual(vcProject.Summary, earlgreyProject);
            
            Assert.IsNotNull(vcProject.Details);
            Assert.AreEqual(2, vcProject.Details.Platforms.Length); // WIN32, x64
            Assert.AreEqual(4, vcProject.Details.Configurations.Length);
        }

        private static Project GetEarlgreyProject()
        {
            SolutionFile slnFile = SolutionFile.FromFile(SolutionFilePath);

            var result = from project in slnFile.Projects
                         where project.ProjectName == "Earlgrey"
                         select project
                ;

            var earlgreyProject = result.First();
            Assert.IsNotNull(earlgreyProject);
            return earlgreyProject;
        }

        [Test]
        public void MergeRootFiles()
        {
            Project earlgreyProject = GetEarlgreyProject();

            var vcProject = new VcProject(earlgreyProject);
            vcProject.Initialize();

            var details = vcProject.Details;


            // string srcFilePath = Path.Combine(ThisDir, "TextFileAnsiEncoded1.txt");
            string dstFilePath = Path.Combine(TempDir, "VcProjectTest.MergeRootFiles.cpp");

            // Assert.IsTrue(File.Exists(srcFilePath));

            if (File.Exists(dstFilePath))
                File.Delete(dstFilePath);

            string projectDir = Path.GetDirectoryName(earlgreyProject.FullPath);
            Assert.IsNotNull(projectDir);
            Assert.IsTrue(Directory.Exists(projectDir));

            using (FileStream dst = File.Open(dstFilePath, FileMode.Create))
            {
                var rootFiles = from file in details.Files
                             where file is FileType
                             select (FileType)file
                ;
                
                foreach(var file in rootFiles)
                {
                    string filePath = Path.GetFullPath(Path.Combine(projectDir, file.RelativePath));
                    Assert.IsTrue(File.Exists(filePath));

                    using (FileStream src = File.Open(filePath, FileMode.Open))
                    {
                        StreamAppend append = new StreamAppend(src, dst);
                        append.Merge();
                    }   
                }
                
            }

            Assert.IsTrue(File.Exists(dstFilePath));
        }
    }
}
