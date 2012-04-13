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
    public class VcProjectMergeTest : AbstractTest
    {
        public override void TearDown()
        {
            base.TearDown();

            foreach (string file in MergedSourceFiles)
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

            VcProjectMerge instance = new VcProjectMerge(vcProject, new ProjectConfigurationNameConverter());
            List<IFilterOrFile> filterOrFiles = instance.Merge();

            Assert.IsNotNull(MergedSourceFiles);
            Assert.Greater(MergedSourceFiles.Length, 3);

            IEnumerable<IFilterType> filtersAdded
               = filterOrFiles.Where(item => item is IFilterType).Select(item => (IFilterType)item);

            IEnumerable<IFileType> filesAdded
                = filterOrFiles.Where(item => item is IFileType).Select(item => (IFileType)item);

            Assert.Greater(filterOrFiles.Count, 3);

            Assert.IsTrue(
                filtersAdded.All(item => item.Name == "UnityBuild")
                );

            CollectionAssert.AllItemsAreUnique(filesAdded);

            Assert.IsTrue(
               filesAdded.All(file => file.Name.StartsWith("UnityBuild-"))
               );
            Assert.IsTrue(
                filesAdded.All(file => file.IsSrcFile == true)
                );
        }
    }
}
