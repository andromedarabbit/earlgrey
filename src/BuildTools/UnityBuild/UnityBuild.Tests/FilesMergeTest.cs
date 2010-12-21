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
            VcProject vcProject = GetEarlgreyVcProject();

            var sourceFilesFilter = FindFilter(vcProject, "Source Files");


            var sourceFiles = from item in sourceFilesFilter.Items
                              where item is FileType
                              select (FileType) item
                              ;

            FilterType newFilter = new FilterType();
            newFilter.Name = "UnityBuild";
            newFilter.NameSpecified = true;
            
            FilesMerge instance = new FilesMerge(vcProject.Summary, newFilter, sourceFiles.ToList());
            instance.Merge();

            Assert.Greater(newFilter.Items.Count, 1);
        }
    }
}
