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
    public class FilesMergeTest : AbstractTest
    {
        [Test]
        public void Merge()
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
                               && ((FilterType) item).Name == "Source Files"
                         select (FilterType) item
                ;


            var sourceFiles = from item in sourceFilesFilter.First().Items
                              where item is FileType
                              select (FileType) item
                              ;

            FilterType newFilter = new FilterType();
            newFilter.Name = "UnityBuild";
            
            FilesMerge instance = new FilesMerge(earlgreyProject, newFilter, sourceFiles.ToList());
            instance.Merge();

            Assert.Greater(newFilter.Items.Count, 1);
        }
    }
}
