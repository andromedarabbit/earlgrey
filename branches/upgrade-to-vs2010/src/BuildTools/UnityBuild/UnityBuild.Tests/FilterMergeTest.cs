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

        private string[] MergedSourceFiles
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
            var sourceFilesFilter = FindFilter(vcProject, "Source Files");

            FilterMerge instance = new FilterMerge(vcProject.Directory, sourceFilesFilter, vcProject.ConfigurationPlatformNames);
            List<IFilterOrFile> filterOrFiles = instance.Merge();


            string dir = Path.GetDirectoryName(vcProject.FullPath);
            string[] filesFound = Directory.GetFiles(dir, "UnityBuild-*cpp", SearchOption.AllDirectories);

            Assert.IsNotNull(filesFound);
            Assert.Greater(filesFound.Length, 3);
        }

        [Test]
        public void CheckAddedItems()
        {
            VcProject vcProject = GetEarlgreyVcProject();
            var sourceFilesFilter = FindFilter(vcProject, "Source Files");

            FilterMerge instance = new FilterMerge(vcProject.Directory, sourceFilesFilter, vcProject.ConfigurationPlatformNames);
            List<IFilterOrFile> filterOrFiles = instance.Merge();

            Assert.Greater(filterOrFiles.Count, 3);


            IEnumerable<FilterType> filtersAdded 
                = filterOrFiles.Where(item => item is FilterType).Select(item => (FilterType)item);

            IEnumerable<FileType> filesAdded
                = filterOrFiles.Where(item => item is FileType).Select(item => (FileType)item);


            Assert.IsTrue(
                filtersAdded.All(item => item.Name == "UnityBuild")
                );

            CollectionAssert.AllItemsAreUnique(filesAdded);

            Assert.IsTrue(
               filesAdded.All(file => file.FileName.StartsWith("UnityBuild-"))
               );
            Assert.IsTrue(
                filesAdded.All(file => file.IsSrcFile == true)
                );
        }
    }
}
