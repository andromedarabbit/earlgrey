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
        [Test]
        public void ReadProjectDetails()
        {
            Project earlgreyProject = GetEarlgreyProject();

            var vcProject = new VcProject(earlgreyProject);
            vcProject.Load();

            Assert.IsNotNull(vcProject.Summary);
            Assert.AreEqual(vcProject.Summary, earlgreyProject);

            Assert.IsNotNull(vcProject.Details);
            Assert.AreEqual(2, vcProject.Details.Platforms.Count); // WIN32, x64
            Assert.AreEqual(4, vcProject.Details.Configurations.Count);
        }

     
        [Test]
        public void MergeRootFiles()
        {
            Project earlgreyProject = GetEarlgreyProject();

            var vcProject = new VcProject(earlgreyProject);
            vcProject.Load();

            var details = vcProject.Details;

            string projectDir = Path.GetDirectoryName(earlgreyProject.FullPath);
            Assert.IsNotNull(projectDir);
            Assert.IsTrue(Directory.Exists(projectDir));

            IEnumerable<object> filesAndFilters = details.Files;
            MergeRootSrcFiles(projectDir, filesAndFilters);
        }

        private static void MergeRootSrcFiles(string projectDir, IEnumerable<object> filesAndFilters)
        {
            var parentFilters = new LinkedList<FilterType>();
            MergeRootSrcFiles(projectDir, filesAndFilters, parentFilters);
        }

        private static void MergeRootSrcFiles(string projectDir, IEnumerable<object> filesAndFilters, IEnumerable<FilterType> parentFilters)
        {
            if (filesAndFilters == null || filesAndFilters.Count() == 0)
                return;

            // string 
            NumberProvider.Instance.Next();
            string dstFileName = string.Format("UnityBuild-{0}.cpp", NumberProvider.Instance.NoString);
            string dstFilePath = Path.Combine(TempDir, dstFileName);

            if (File.Exists(dstFilePath))
                File.Delete(dstFilePath);

            using (SrcFileAppend merger = new SrcFileAppend(dstFilePath, projectDir, true))
            {
                merger.Open();

                foreach (var fileOrFilter in filesAndFilters)
                {
                    Assert.IsNotNull(fileOrFilter);

                    Assert.IsTrue(
                        (fileOrFilter is FileType) || (fileOrFilter is FilterType)
                        );

                    if (fileOrFilter is FileType)
                    {
                        FileType file = (FileType)fileOrFilter;
                        if (file.IsSrcFile == false)
                            continue;

 //                       // TODO: 임시 코드
 //                       if (file.RelativePath.EndsWith("NotUsed.cpp"))
 //                       {    
 //bool d=file.IsExcludedFromBuild();
 //                       }

                        merger.MergeSrcFile(file);
                    }

                    if (fileOrFilter is FilterType)
                    {
                        FilterType filter = (FilterType)fileOrFilter;

                        var newParentFilters = new LinkedList<FilterType>(parentFilters);
                        newParentFilters.AddLast(filter);

                        MergeRootSrcFiles(projectDir, filter.Items, newParentFilters);
                    }
                }
            }
        }

        // End of MergeRootFiles
    }
}
