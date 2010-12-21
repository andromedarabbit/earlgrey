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
    public class FilterMergeTest : AbstractTest
    {
        public override void TearDown()
        {
            base.TearDown();

            foreach(string file in MergedSourceFiles)
            {
                File.Delete(file);
            }
        }

        private static string[] MergedSourceFiles
        {
            get
            {
                Project earlgreyProject = GetEarlgreyProject();
                string dir = Path.GetDirectoryName(earlgreyProject.FullPath);
                return Directory.GetFiles(dir, "UnityBuild-*cpp", SearchOption.AllDirectories);
            }
        }

        [Test]
        public void MergeFilterSourceFiles()
        {
            VcProject vcProject = GetEarlgreyVcProject();

            var details = vcProject.Details;

            string projectDir = Path.GetDirectoryName(vcProject.Summary.FullPath);
            Assert.IsNotNull(projectDir);
            Assert.IsTrue(Directory.Exists(projectDir));

            IEnumerable<object> filesAndFilters = details.Files;
            var sourceFilesFilter = FindFilter(vcProject, "Source Files");

            FilterMerge instance = new FilterMerge(vcProject.Summary, sourceFilesFilter);
            instance.Merge();

            Assert.IsNotNull(MergedSourceFiles);
            Assert.Greater(MergedSourceFiles.Length, 3);
        }
    }
}
