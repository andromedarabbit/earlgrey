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
            Project earlgreyProject = GetEarlgreyProject();

            var vcProject = new VcProject(earlgreyProject);
            vcProject.Load();

            var details = vcProject.Details;

            string projectDir = Path.GetDirectoryName(earlgreyProject.FullPath);
            Assert.IsNotNull(projectDir);
            Assert.IsTrue(Directory.Exists(projectDir));

            IEnumerable<object> filesAndFilters = details.Files;
            var sourceFilesFilter = from item in filesAndFilters
                                    where item is FilterType
                                          && ((FilterType)item).Name == "Source Files"
                                    select (FilterType)item
                ;

            FilterMerge instance = new FilterMerge(earlgreyProject, sourceFilesFilter.First());
            instance.Merge();

            Assert.IsNotNull(MergedSourceFiles);
            Assert.Greater(MergedSourceFiles.Length, 3);
        }
    }
}
