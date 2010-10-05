﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class SrcFileAppendTest : AbstractTest
    {
        private static readonly string SolutionFilePath;

        // SolutionReaderTest 와 동일하다.
        static SrcFileAppendTest()
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

            string dstFilePath = Path.Combine(TempDir, "VcProjectTest.MergeRootFiles.cpp");

            if (File.Exists(dstFilePath))
                File.Delete(dstFilePath);

            string projectDir = Path.GetDirectoryName(earlgreyProject.FullPath);
            Assert.IsNotNull(projectDir);
            Assert.IsTrue(Directory.Exists(projectDir));

            object[] filesAndFilters = details.Files;     
            MergeRootSrcFiles(dstFilePath, projectDir, filesAndFilters);
        }

        private static void MergeRootSrcFiles(string dstFilePath, string projectDir, object[] filesAndFilters)
        {
            if (filesAndFilters == null || filesAndFilters.Length == 0)
                return;

            using (SrcFileAppend merger = new SrcFileAppend(dstFilePath, projectDir, true))
            {
                merger.Open();

                foreach (var fileOrFilter in filesAndFilters)
                {
                    Assert.IsTrue(
                        (fileOrFilter is FileType) || (fileOrFilter is FilterType)
                        );

                    if (fileOrFilter is FileType)
                    {
                        FileType file = (FileType)fileOrFilter;
                        if (SrcFileAppend.IsSrcFile(file) == false)
                            continue;

                        merger.MergeSrcFile(file);
                    }

                    if (fileOrFilter is FilterType)
                    {
                        FilterType filter = (FilterType)fileOrFilter;

                        string newDstFilePath = Path.Combine(TempDir, "VcProjectTest.MergeRootFiles." + filter.Name + ".cpp");
                        MergeRootSrcFiles(newDstFilePath, projectDir, filter.Items);
                    }
                }
            }
        }

        // End of MergeRootFiles
    }
}
