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
                              where item is IFileType
                              select (IFileType) item
                              ;
            
            // FilesMerge instance = new FilesMerge(vcProject.Summary, newFilter, sourceFiles.ToList());
            FilesMerge instance = new FilesMerge(vcProject.Directory, sourceFiles.ToList(), vcProject.ConfigurationPlatformNames);
            List<IFileType> filesAdded = instance.Merge();

            Assert.GreaterOrEqual(filesAdded.Count, 1);
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
