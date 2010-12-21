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

            VcProjectMerge instance = new VcProjectMerge(vcProject);
            instance.Merge();

            Assert.IsNotNull(MergedSourceFiles);
            Assert.Greater(MergedSourceFiles.Length, 3);
        }
    }
}
